
# wmbusmeters

The program acquires utility meter readings from wired m-bus or
wireless wm-bus meters.  The readings can then be published using
MQTT, curled to a REST api, inserted into a database or stored in a
log file.

# What does it do?

Wmbusmeters converts incoming telegrams from (w)mbus/OMS compatible meters like:
`1844AE4C4455223368077A55000000_041389E20100023B0000`

into human readable:
`MyTapWater  33225544  123.529 m³  0 m³/h  2024-03-03 19:36:22`

or into csv:
`MyTapWater;33225544;123.529;0;2024-03-03 19:36:45`

or into json:
```json
{
    "_":"telegram",
    "media":"water",
    "meter":"iperl",
    "name":"MyTapWater",
    "id":"33225544",
    "max_flow_m3h":0,
    "total_m3":123.529,
    "timestamp":"2024-03-03T18:37:00Z"
}
```

Wmbusmeters can collect telegrams from radio using hardware dongles or
rtl-sdr software radio dongles, or from m-bus meters using serial
ports, or from files/pipes.

Wmbusmeters uses drivers to translate the measurement values from a
meter to understandable key-value pairs.

You can automatically decode a 32 bit signed binary word
representing a volume in m3 stored in storage slot 1 but what does this
measurement mean? Is it last months final measurement that will be
used for water billing? Is it the amount of gas delivered last year?
The [drivers](https://github.com/wmbusmeters/wmbusmeters/tree/master/drivers/src)
adds this knowledge, here is an example:
```
field {
    name = target
    info = 'Water consumption at the end of last month.'
    quantity = Volume
    match {
        measurement_type = Instantaneous
        vif_range = Volume
        storage_nr = 20
    }
}
```

[FAQ/WIKI/MANUAL pages](https://wmbusmeters.github.io/wmbusmeters-wiki/)

The program runs on GNU/Linux, MacOSX, FreeBSD, and Raspberry Pi.

| System       | Status        |
| ------------ |:-------------:|
| Ubuntu | [![Build Ubuntu Status](https://github.com/wmbusmeters/wmbusmeters/workflows/Build%20Ubuntu/badge.svg)](https://github.com/wmbusmeters/wmbusmeters/actions)|
| MacOSX | [![Build MacOSX Status](https://github.com/wmbusmeters/wmbusmeters/workflows/Build%20MacOSX/badge.svg)](https://github.com/wmbusmeters/wmbusmeters/actions)|
| Docker | [![Build Docker Status](https://github.com/wmbusmeters/wmbusmeters/workflows/Build%20docker/badge.svg)](https://hub.docker.com/r/wmbusmeters/wmbusmeters/)|
| Snap | [![Build Snap Status](https://github.com/wmbusmeters/wmbusmeters/workflows/Build%20Snap/badge.svg)](https://snapcraft.io/wmbusmeters)|

# Distributions

**wmbusmeters** package is available on [Fedora](https://src.fedoraproject.org/rpms/wmbusmeters) _(version 31 or newer)_ and can be simply installed by using:

```shell
dnf install wmbusmeters
```

**wmbusmeters** for [Debian](https://tracker.debian.org/pkg/wmbusmeters) is currently available through Experimental repositories. Availability for other Linux distributions can be checked on [release-monitoring](https://release-monitoring.org/project/88654/) project page.

# Docker

Experimental docker containers are available here: https://hub.docker.com/r/wmbusmeters/wmbusmeters

# Snap

Experimental snaps are available here: https://snapcraft.io/wmbusmeters
Read the wiki for more info on how to use the snap: https://wmbusmeters.github.io/wmbusmeters-wiki/SNAP.html

# Build from source and run as a daemon

Building and installing from source is easy and recommended since the
development progresses quickly.  First remove the wmbus dongle
(im871a,iu891a,amb8465(metis),amb3665,cul,rc1180) or the generic rtlsdr dongle (RTL2832U)
from your computer. Then do:

`./configure; make; sudo make install` will install wmbusmeters as a daemon.

# Usage

Check the contents of your `/etc/wmbusmeters.conf` file, assuming it
has `device=auto:t1` and you are using a im871a,iu891a,amb8465(metis),amb3665,rc1180,cul or rtlsdr device,
then you can now start the daemon with `sudo systemctl start wmbusmeters`
or you can try it from the command line `wmbusmeters auto:t1`

Wmbusmeters will scan for wmbus devices every few seconds and detect whenever
a device is plugged in or removed. However since wmbusmeters now supports
several dongle types, the scan can take some time!

Use `auto` for testing and to find your dongle. For production it is very much
recommended that you change `auto:t1` to the device name with the full device path
(eg `/dev/ttyAMA0:iu891a:c1,t1`). This will skip the slow probing for all possible
wmbus dongles when wmbusmeters startup.

If the serial device (ttyUSB0) might change you can also use `device=iu891a:c1,t1`
which will probe all serial devices but only scans for im891a which also speeds it up.

Note that the rtl-sdr devices are not found under the tty devices (e.g. `/dev/tty...`).
Instead the rtl-sdr devices are accessed through character device special files named `/dev/swradio0` to `/dev/swradio255`[^kernel_docs_sdr]. Wmbusmeters uses librtsldr to probe these devices.

[^kernel_docs_sdr]: https://docs.kernel.org/userspace-api/media/v4l/dev-sdr.html?highlight=sdr#software-defined-radio-interface-sdr

If you have to scan serial devices, then remember that some Raspberry PIs are upset when
random data is sent to `/dev/ttyAMA0` when it is configured in bluetooth mode.
To solve this, add `donotprobe=/dev/ttyAMA0`

To have the wmbusmeters daemon start automatically when the computer boots do:
`sudo systemctl enable wmbusmeters`

You can trigger a reload of the config files with `sudo killall -HUP wmbusmetersd`

(Note! make install only works for GNU/Linux. For MacOSX try to start
`wmbusmetersd /tmp/thepidfile` from a script instead.)

You can also start the daemon with another set of config files:
`wmbusmetersd --useconfig=/home/me/.config/wmbusmeters /tmp/thepidfile`

When using useconfig, the files/dir should be:
`/home/me/.config/wmbusmeters/wmbusmeters.conf` and the meters dir:
`/home/me/.config/wmbusmeters/wmbusmeters.d`

Check the config file /etc/wmbusmeters.conf and edit the device. For example:
`/dev/ttyUSB1:amb8465:c1,t1` or `iu891a:c1,t1` or `iu891a[457200101056]:t1`.

Adding a device like auto or iu891a will trigger an automatic probe of all serial ttys
to auto find or to find on which tty the iu891a resides.

If you specify a full device path like `/dev/ttyUSB0:iu891a:c1` or `rtlwmbus` or `rtl433`
then it will not probe the serial devices. If you must be really sure that it will not probe something
you can add `donotprobe=/dev/ttyUSB0` or `donotprobe=all`.

You can specify combinations like: `device=rc1180:t1` `device=auto:c1`
to set the rc1180 dongle to t1 but any other auto-detected dongle to c1.

Some dongles have identifiers (im871a,iu891a,amb8465(metis),amb3665 and rtlsdrs) (for example: rtlsdr can be set with `rtl_eeprom -s myname`)
You might have two rtlsdr dongles, one attached to an antenna tuned to 433MHz and the other
attached to an antenna tuned for 868.95MHz, then a more complicated setup could look like this:

```
device=rtlwmbus[555555]:433M
device=rtlwmbus[112233]
device=/dev/ttyUSB0:iu891a[00102759]:c1,t1
device=/dev/ttyUSB1:rc1180:t1
```

# Bus aliases and polling

To poll an C2/T2/S2 wireless meter or an wired m-bus meter you need to give the (w)mbus device a bus-alias, for example
here we pick the bus alias MAIN for the mbus using 2400 bps for all meters on this bus.
```
MAIN=/dev/ttyUSB0:mbus:2400
```
and here we pick the bus alias RADIOMAIN for an iu891a dongle:
```
RADIOMAIN=/dev/ttyUSB1:iu891a:c2
```

The bus alias is then used in the meter driver specification to specify which
bus the mbus poll request should be sent to.
```
wmbusmeters --pollinterval=60s MAIN=/dev/ttyUSB0:mbus:2400 MyTempMeter piigth:MAIN:mbus 12001932 NOKEY
```

If you want to poll an mbus meter using the primary address, use p0 to p250 (deciman numbers)
instead of the full 8 digit secondary address.
```
wmbusmeters --pollinterval=60s MAIN=/dev/ttyUSB0:mbus:2400 MyTempMeter piigth:MAIN:mbus p0 NOKEY
```

# Example wmbusmeter.conf file

```ini
loglevel=normal
# You can use auto:t1 to find the device you have connected to your system.
# But do not use auto here since it will cause unnecessary and slow probing of the serial ports.
device=/dev/ttyUSB0:iu891a:c1,t1
# And mbus
device=MAIN=/dev/ttyUSB1:mbus:2400
# But do not probe this serial tty.
donotprobe=/dev/ttyACM2
logtelegrams=false
format=json
meterfiles=/var/lib/wmbusmeters/meter_readings
meterfilesaction=overwrite
meterfilesnaming=name
meterfilestimestamp=day
logfile=/var/log/wmbusmeters/wmbusmeters.log
shell=/usr/bin/mosquitto_pub -h localhost -t wmbusmeters/$METER_ID -m "$METER_JSON"
alarmshell=/usr/bin/mosquitto_pub -h localhost -t wmbusmeters_alarm -m "$ALARM_TYPE $ALARM_MESSAGE"
alarmtimeout=1h
alarmexpectedactivity=mon-sun(00-23)
ignoreduplicates=true
```

Then add a meter file in /etc/wmbusmeters.d/MyTapWater

```ini
name=MyTapWater
id=12345678
key=00112233445566778899AABBCCDDEEFF
driver=multical21
```

And an mbus meter file in /etc/wmbusmeters.d/MyTempHygro

```ini
name=MyTempHygro
id=11223344
driver=piigth:MAIN:mbus
pollinterval=60s
```

# Important information about meter drivers and their names.

You can use `driver=auto` to have wmbusmeters automatically detect
and use the best driver for your meter, but you should >not< use auto in production.

You can find out which driver is recommended by running `wmbusmeters iu891a:t1`.
This will print information like:
```
Received telegram from: 71727374
          manufacturer: (BMT) BMETERS, Italy (0x9b4)
                  type: Heat/Cooling load meter (0x0d) encrypted
                   ver: 0x0b
                driver: hydrocalm3
```

For production use it is very much recommended that you specify the exact driver
in the meter file. The reason is that new and better drivers might be developed
for your meter, where the keys and the content of the json might change.
Such new drivers are guaranteed to have a different driver name.
The auto look up will change to the new driver, but the old driver will still work.

So wmbusmeters strives to guarantee that if you have specified the driver name,
then wmbusmeters can be safely upgraded at any time. The json will not
change in an incompatible way. (The only allowed changes are: adding new fields
and changing the ordering.)

Now plugin your wmbus dongle.

Wmbusmeters should start automatically, check with `tail -f /var/log/syslog` and `tail -f /var/log/wmbusmeters/wmbusmeters.log`
(If you are using an rtlsdr dongle, then make sure that either the binaries `/usr/bin/rtl_sdr` and
`/usr/bin/rtl_wmbus` exists and are executable. Or that the executable `rtl_sdr/rtl_wmbus` binaries
exists inside the same directory as the wmbusmeters executable. If not you will see the
error message `(rtlwmbus) error: when starting as daemon, wmbusmeters looked for .../rtl_wmbus and /usr/bin/rtl_wmbus, but found neither!`
and the daemon will refuse to start.)

The latest reading of the meter can also be found here: `/var/lib/wmbusmeters/meter_readings/MyTapWater`

You can use several ids using `id=1111111,2222222,3333333` or you can listen to all
meters of a certain type `id=*` or you can suffix with star `id=8765*` to match
all meters with a given prefix. If you supply at least one positive match rule, then you
can add filter out rules as well. For example `id=*,!2222*`
which will match all meter ids, except those that begin with 2222.

You can also specify the exact manufacturer, version and type: `id=11111111.M=KAM.V=1b.T=16`
or a subset: `id=11111111.T=16` or all telegrams from 22222222 except those with version 77:
`id=22222222,!22222222.V=77` You can also use the fully specified secondary address that is
printed by libmbus after doing a bus scan, ie `100002842941011B` which is equivalent to
`10000284.M=PII.V=01.T=1B`

When matching all meters from the command line you can use `ANYID` instead of `*` to avoid shell quotes.

# Add static and calculated fields to the output

You can add the static json data `"address":"RoadenRd 456","city":"Stockholm"` to every json message with the
wmbusmeters.conf setting:

```ini
field_address=RoadenRd 456
field_city=Stockholm
```

If you add `field_floor=5` to the meter file `MyTapWater`, then you can have the meter tailored static json `"floor":"5"` added to telegrams handled by that particular meter. (The old prefix json_ still works.)

You can add unit conversions and calculated values to the meter files using
`calculate_...`.  The formulas track units. If the unit do not match
up, then the formula will generate a null value. When two units are compatible
it will automatically convert the value between two units.

The formula
```ini
calculate_sum_mj=5 kwh + 8 gj + (7 kw * 3 h)
```

will add the field:

```json
"sum_mj":8093.6
```

Units inside the formula calculation are tracked as arbitrary SI unit
exponents (ie Volt is `1kgm²s⁻³a⁻¹`) however the final result must be
a named unit (ie the calculated field must end with `_v`). The
existing named units can be found with `wmbusmeters --listunits`.

If you make a mistake in the formula you will get a warning:
```
Warning! Ignoring calculated field sum because parse failed:
Cannot add [kw|Power|1000kgm²s⁻³] to [gj|Energy|1×10⁹kgm²s⁻²]!
5 kw + 8 gj + (7 kw * 3 h)
     ^~~~~
```

You need parentheses in the formulas since operator precedence is not yet implemented.

```ini
calculate_total_l=total_m3
calculate_approx_power_m3ch=(t1_temperature_c-t2_temperature_c)*volume_flow_m3h
calculate_total_mj=total_energy_consumption_kwh
```

```
wmbusmeters --format=json --ppjson
--field_collector=cm57829
--calculate_total_l=total_volume_m3
--calculate_approx_power_m3ch='(t1_temperature_c-t2_temperature_c)*volume_flow_m3h'
--calculate_total_mj=total_energy_consumption_kwh
5e442d2c1155775540047a7d0050252f2f0406c50e000004147B86000004ff074254000004ff086047000002594117025d9a14023Bed0302ff220000026cca2c4406750B00004414ad680000426cc12c2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f
Heato kamheat 55775511 NOKEY
```

which will output:

```json
{
    "media":"heat",
    "meter":"kamheat",
    "name":"Heato",
    "id":"55775511",
    "status":"OK",
    "total_energy_consumption_kwh":3781,
    "total_volume_m3":344.27,
    "volume_flow_m3h":1.005,
    "t1_temperature_c":59.53,
    "t2_temperature_c":52.74,
    "forward_energy_m3c":21570,
    "return_energy_m3c":18272,
    "meter_date":"2022-12-10",
    "target_energy_kwh":2933,
    "target_volume_m3":267.97,
    "target_date":"2022-12-01",
    "total_l":344270,
    "approx_power_m3ch":6.82395,
    "total_mj":13611.6,
    "timestamp":"2023-01-14T07:20:22Z",
    "collector":"cm57829"
}
```

If you have connected your Lansen pulse counting meter to an
electricity meter triggering a pulse per 0.1 kwh, then you can
directly calculate a value based on the counters:

```
wmbusmeters --format=json --ppjson
--calculate_total_kwh='1000 kwh + (a_counter * 0.1 kwh)'
234433300602010014007a8e0400002f2f0efd3a1147000000008e40fd3a341200000000
Electricity lansenpu 00010206 NOKEY
```

```json
{
    "media":"other",
    "meter":"lansenpu",
    "name":"Electricity",
    "id":"00010206",
    "status":"POWER_LOW",
    "a_counter":4711,
    "b_counter":1234,
    "total_kwh":1471.1,
    "timestamp":"2023-01-14T07:25:34Z"
}
```

# Miscellaneous

If you are running on a Raspberry PI with flash storage and you relay
the data to another computer using a shell command (`mosquitto_pub` or
`curl` or similar) then you might want to remove `meterfiles` and
`meterfilesaction` to minimize the writes to the local flash file
system.

Also when using the Raspberry PI it can get confused by the serial ports, in particular the bluetooth port might come and
go as a serial tty depending on the config. Therefore it can be advantageous to use the auto device to find the proper tty
(eg /dev/ttyUSB0) and then specify this tty device explicitly in the config file, instead of using auto. This assumes that
you only have a single usb dongle otherwise the USB tty names can change depending on how and when the devices are
unplugged/replugged and the pi restarted. If you have multiple devies with different antennas, then you should instead
use donotprobe to avoid the ttys that can never have a wmbus dongle.

If you specify `--meterfilesaction=append --meterfilestimestamp=day` then wmbusmeters will append all todays received telegrams in for example the file `Water_2019-12-11`, the day after the telegrams will be recorded in `Water_2019-12-12`. You can change the resolution to day,hour,minute and micros. Micros means that every telegram gets their own file.

The purpose of the alarm shell and timeout is to notify you about
problems within wmbusmeters and the wmbus dongles, not the meters
themselves. Thus the timeout is for a dongle to receive some telegram
at all. It does not matter from which meter.

# Run using config files

If you cannot install as a daemon, then you can also start
wmbusmeters in your terminal using the config files in `/etc/wmbusmeters`.

```shell
wmbusmeters --useconfig=/etc
```

Or you can start wmbusmeters with your own config files:

```shell
wmbusmeters --useconfig=/home/me/.config/wmbusmeters
```

If you already have config with a device specified, and you want to use
the config with another device. You might have multiple meters in the config
that you want to listen to. Then you can add `--overridedevice` to override the settings
in the config. Like this:

```shell
wmbusmeters --useconfig=/home/me/.config/wmbusmeters --overridedevice=rtlwmbus
```

You must have both `--useconfig=` and `--overridedevice=` for it to work.

The files/dir should then be located here:
`/home/me/.config/wmbusmeters/wmbusmeters.conf` and
`/home/me/.config/wmbusmeters/wmbusmeters.d`

The option `--useconfig=` can only be combined with a few other options: `--overridedevice= --listento= --exitafter= --oneshot= --silent --normal --verbose --debug --trace`

When running using config files then you can trigger a reload of the config files
using `sudo killall -HUP wmbusmetersd` or `killall -HUP wmbusmeters`
depending on if you are running as a daemon or not.

# Running without config files, good for experimentation and test.

```
wmbusmeters version: 1.19.0
Usage: wmbusmeters {options} [device] { [meter_name] [meter_driver] [meter_id] [meter_key] }*
       wmbusmeters {options} [hex]    { [meter_name] [meter_driver] [meter_id] [meter_key] }*
       wmbusmetersd {options} [pid_file]

As {options} you can use:

    --alarmexpectedactivity=mon-fri(08-17),sat-sun(09-12) Specify when the timeout is tested, default is mon-sun(00-23)
    --alarmshell=<cmdline> invokes cmdline when an alarm triggers
    --alarmtimeout=<time> Expect a telegram to arrive within <time> seconds, eg 60s, 60m, 24h during expected activity.
    --analyze Analyze a telegram to find the best driver.
    --analyze=<key> Analyze a telegram to find the best driver use the provided decryption key.
    --analyze=<driver> Analyze a telegram and use only this driver.
    --analyze=<driver>:<key> Analyze a telegram and use only this driver with this key.
    --calculate_field_unit='...' Add field_unit to the json and calculate it using the formula. E.g.
    --calculate_sumtemp_c='external_temperature_c+flow_temperature_c'
    --calculate_flow_f=flow_temperature_c
    --debug for a lot of information
    --donotprobe=<tty> do not auto-probe this tty. Use multiple times for several ttys or specify "all" for all ttys.
    --driver=<file> load a driver
    --driversdir=<dir> load all drivers in dir
    --exitafter=<time> exit program after time, eg 20h, 10m 5s
    --format=<hr/json/fields> for human readable, json or semicolon separated fields
    --help list all options
    --identitymode=(id|id-mfct|full|none) group meter state based on the identity mode. Default is id.
    --ignoreduplicates=<bool> ignore duplicate telegrams, remember the last 10 telegrams
    --field_xxx=yyy always add "xxx"="yyy" to the json output and add shell env METER_xxx=yyy (--json_xxx=yyy also works)
    --license print GPLv3+ license
    --listento=<mode> listen to one of the c1,t1,s1,s1m,n1a-n1f link modes
    --listento=<mode>,<mode> listen to more than one link mode at the same time, assuming the dongle supports it
    --listenvs=<meter_driver> list the env variables available for the given meter driver
    --listfields=<meter_driver> list the fields selectable for the given meter driver
    --printdriver=<meter_driver> print xmq driver source code
    --listmeters list all meter drivers
    --listmeters=<search> list all meter drivers containing the text <search>
    --listunits list all unit suffixes that can be used for typing values
    --logfile=<file> use this file for logging or --logfile=syslog
    --logtelegrams log the contents of the telegrams for easy replay
    --logtimestamps=<when> add log timestamps: always never important
    --meterfiles=<dir> store meter readings in dir
    --meterfilesaction=(overwrite|append) overwrite or append to the meter readings file
    --meterfilesnaming=(name|id|name-id) the meter file is the meter's: name, id or name-id
    --meterfilestimestamp=(never|day|hour|minute|micros) the meter file is suffixed with a
                          timestamp (localtime) with the given resolution.
    --metershell=<cmdline> invokes cmdline with env variables the first time a meter is seen since startup
    --nodeviceexit if no wmbus devices are found, then exit immediately
    --normal for normal logging
    --oneshot wait for an update from each meter, then quit
    --overridedevice=<device> override device in config files. Use only in combination with --useconfig= option
    --ppjson pretty print the json
    --pollinterval=<time> time between polling of meters, must be set to get polling.
    --resetafter=<time> reset the wmbus dongle regularly, default is 23h
    --selectfields=id,timestamp,total_m3 select only these fields to be printed (--listfields=<meter> to list available fields)
    --separator=<c> change field separator to c
    --shell=<cmdline> invokes cmdline with env variables containing the latest reading
    --silent do not print informational messages nor warnings
    --trace for tons of information
    --useconfig=<dir> load config <dir>/wmbusmeters.conf and meters from <dir>/wmbusmeters.d
    --usestderr write notices/debug/verbose and other logging output to stderr (the default)
    --usestdoutforlogging write debug/verbose and logging output to stdout
    --verbose for more information
    --version print version
```

As device you can use:

`auto:c1`, to have wmbusmeters probe for devices: im871a, iu891a, amb8465(metis), amb3665, cul, rc1180 or rtlsdr (spawns rtlwmbus).

`iu891a:c1` to start all connected *iu891a* devices in *c1* mode, ignore all other devices.

`/dev/ttyUSB1:amb8465:c1` to start only this device on this tty. Do not probe for other devices.

If you have two iu891a you can supply both of them with their unique id:s and set different listening modes:
`iu891a[12345678]:c1` `iu891a[11223344]:t1`

You can also specify rtlwmbus and if you set the serial in the rtlsdr
dongle using `rtl_eeprom -s 1234` you can also refer to a specific
rtlsdr dongle like this `rtlwmbus[1234]`.

`/dev/ttyUSB0:amb8465`, if you have an amb8465(metis) dongle assigned to ttyUSB0. Other suffixes are iu891a,cul.

(Note that a plain `/dev/ttyUSB0` no longer works, you have to specify the device expected on the device.)

`/dev/ttyUSB0:38400`, to have wmbusmeters set the baud rate to 38400 and listen for raw wmbus telegrams.
These telegrams are expected to have the data link layer crc bytes removed already!

`MAIN=/dev/ttyUSB0:mbus:2400`, assume ttyUSB0 is an serial to mbus-master converter.  The speed is set to 2400 bps.

`rtlwmbus`, to spawn the background process: `rtl_sdr -f 868.625M -s 1600000 - 2>/dev/null | rtl_wmbus -f -s`
for each attached rtlsdr dongle. This will listen to S1,T1 and C1 meters in parallel.

For the moment, it is necessary to send the stderr to a file (/dev/null) because of a bug:
https://github.com/osmocom/rtl-sdr/commit/142325a93c6ad70f851f43434acfdf75e12dfe03

Until this bug fix has propagated into Debian/Fedora etc, wmbusmeters uses a tmp file
to see the stderr output from rtl_sdr. This tmp file is created in /tmp and will
generate 420 bytes of data once ever 23 hours.

The current command line used by wmbusmeters to start the rtl_wmbus pipeline is therefore a bit longer:
```
ERRFILE=$(mktemp --suffix=_wmbusmeters_rtlsdr) ;
echo ERRFILE=$ERRFILE ;  date -Iseconds > $ERRFILE ;
tail -f $ERRFILE & /usr/bin/rtl_sdr  -d 0 -f 868.625M -s 1.6e6 - 2>>$ERRFILE | /usr/bin/rtl_wmbus -s -f
```

Note that the standard -s option uses a noticeable amount of CPU time by rtl_wmbus.
You can therefore use a tailored rtl_wmbus command that is more suitable for your needs.

`rtlwmbus:CMD(<command line>)`, to specify the entire background
process command line that is expected to produce rtlwmbus compatible
output.
The command line cannot contain parentheses.
Likewise for rtl433.

Here is an example command line that reduces the rtl_wmbus CPU usage if you only need T1/C1 telegrams.
It disable S1 decoding (`-p s`) and trades lower cpu usage for reception performance (`-a`).
You should always add the `-f` option to enable detection if rtl_sdr has stalled:

`rtlwmbus:CMD(rtl_sdr -f 868.95M -s 1600000 - 2>/dev/null | rtl_wmbus -p s -a -f)`

`rtlwmbus(ppm=17)`, to tune your rtlsdr dongle accordingly.
Use this to tune your dongle and at the same time listen to S1,T1 and C1.

`rtlwmbus:433M`, to tune to this fq instead.
This will listen to exactly to what is on this frequency.

`rtl433`, to spawn the background process: `rtl_433 -F csv -f 868.95M`

`rtl433(ppm=17)`, to tune your rtlsdr dongle accordingly.

`rtl433:433M`, to tune to this fq instead.

`stdin:rawtty`, to read raw binary telegrams from stdin.
These telegrams are expected to have the data link layer crc bytes removed already!

`telegrams.bin:rawtty`, to read raw wmbus telegrams from this file.
These telegrams are expected to have the data link layer crc bytes removed already!

`stdin:hex`, to read hex characters wmbus telegrams from stdin.
These telegrams are expected to have the data link layer crc bytes removed already!

`telegrams.txt:hex`, to read hex characters wmbus telegrams from this file.
These telegrams are expected to have the data link layer crc bytes removed already!

`stdin:rtlwmbus`, to read telegrams formatted using the rtlwmbus format from stdin. Works for rtl433 as well.

`telegrams.msg:rtlwmbus`, to read rtlwmbus formatted telegrams from this file. Works for rtl433 as well.

`simulation_abc.txt`, to read telegrams from the file (the file must have a name beginning with simulation_....)
expecting the same format that is the output from `--logtelegrams`. This format also supports replay with timing.
The telegrams are allowed to have valid dll crcs, which will be automatically stripped.

As meter quadruples you specify:

* `<meter_name>`: a mnemonic for this particular meter (!Must not contain a colon ':' character!)
* `<meter_driver>`: use `auto` or one of the supported meters (can be suffixed with: `:<bus_alias>` for selecting which bus where we should send the poll requests  `:<mode>` to specify which mode you expect the meter to use when transmitting)
* `<meter_id>`: an 8 digit mbus id, usually printed on the meter
* `<meter_key>`: an encryption key unique for the meter
  if the meter uses no encryption, then supply `NOKEY`

```
Supported wmbus dongles:
IMST 871a (im871a)
IMST 891a (iu891a)
Amber 8465-M/8665-M/8626-M/Metis-II (amb8465) 868MHz
Amber 3665-M (amb3665) 169MHz
CUL family (cul)
Radiocraft (rc1180)
rtl_wmbus (rtlwmbus)
rtl_433 (rtl433)

Supported mbus dongles:
Any bus controller dongle/board behaving like a plain serial port.

Supported water meters:
Aventies (aventieswm)
Apator at-wmbus-08   (apator08) (non-standard protocol)
Apator at-wmbus-08-2   (apator082) (non-standard protocol)
Apator at-wmbus-16-2 (apator162) (non-standard protocol)
Apator at-wmbus-17-2 (apator172) (non-standard protocol)
Apator Ultrimis (ultrimis)
Aquametro/Integra Topas Es Kr (topaseskr)
Axioma W1 (q400)
Bmeters Hydrodigit (hydrodigit) (partly non-standard protocol)
Bmeters GSD8-I with IWM-TX5 module (iwmtx5)
Diehl/Sappel IZAR RC 868 I R4 PL and R3 (izar) (non-standard protocol)
Diehl HYDRUS (hydrus)
Diehl IZAR RC I G4 (dme_07)
Elster Merlin 868 (emerlin868)
Elster V200H (ev200)
GWF Water (gwfwater)
Maddalena EVO 868 (evo868)
Honeywell Q400 (q400)
Itron (itron)
Kamstrup Multical 21 (kamwater)
Kamstrup flowIQ 2200 (kamwater)
Kamstrup flowIQ 3100 (kamwater)
Qundis QWater5.5 (lse_07_17)
Sontex Supercom 587 (supercom587)
Sensus iPERL (iperl)
Techem MK Radio 3 and 4 (mkradio3,mkradio4) (non-standard protocols)
Waterstar M (waterstarm)
Watertech (watertech)
Zenner Minomess (minomess)

Supported heat cost allocators:
Apator E-ITN 30.51 (apatoreitn)
Engelmann HCA e2 (hcae2)
Innotas EurisII  (eurisii)
Qundis Q caloric (qcaloric)
Sontex 868 (sontex868)
Techem FHKV data II/III (fhkvdataiii)
Siemens WHE542 (whe5x)
BMeters Hydroclima RFM (hydroclima)
BFW 240 (bfw240radio)

Supported heat meters:
Heat meter Techem Compact V / Compact Ve (compact5) (non-standard protocol)
Heat meter Techem vario 3 type 3.2.1 (compact5)
Heat meter Techem vario 4 (vario451) (non-standard protocol)
Heat and Cooling meters Kamstrup Multical 302,403,602,603,803 (kamheat)
Heat meter Apator Elf (elf)
Heat meter Enercal F2 (enercal)
Heat meter Diehl Sharky 775 (sharky)
Heat meter Diehl Sharky 774 (sharky774)
Heat meter Maddelena microClima (microclima)
Heat and Cooling meter BMeters Hydrocal-M3 (hydrocalm3)
Heat and Cooling meter Axioma Qualcosonic E3 (qualcosonic)
Heat meter Qundis Q heat 5.5 (qheat)
Heat meter Sensus Pollucom F (pollucomf)

Supported room sensors:
Bmeters RFM-AMB Thermometer/Hygrometer (rfmamb)
Elvaco CMa12w Thermometer (cma12w)
Lansen Thermometer/Hygrometer (lansenth)
Weptech Munia / Robin Thermometer/Hygrometer (munia)
PiiGAB Thermometer/Hygrometer (piigth) wired

Supported smoke detectors:
Lansen Smoke Detector (lansensm)
EI Electronics Smoke Detector ei6500-oms (ei6500)

Supported door/window detectors:
Lansen Door/Window Detector (lansendw)

Supported pulse counter:
Lansen Pulse Counter (lansenpu)

Supported electricity meters:
Easy Meter ESYS-WM20 (esyswm)
eBZ wMB-E01 (ebzwmbe)
EMH Metering (ehzp)
Tauron Amiplus (amiplus) (includes vendor apator and echelon)
Gavazzi EM24 (em24)
Gransystems 301 and 303 (gransystems)
Kamstrup Omnipower (omnipower)

Supported gas meters:
uniSMART (unismart)

Supported pressure sensors:
Kamstrup Pressure Sensor (kampress)


```

The wmbus dongle im871a can listen to either s1, c1 or t1.
With the latest firmware version (0x15) im871a can
also listen to c1 and t1 telegrams at the same time.
(Use `--verbose` to see your dongles firmware version.)
If you have the older firmware you can download the upgrader here:
https://wireless-solutions.de/downloadfile/wireless-m-bus-software/

The wmbus dongle iu891a can listen to either s1, c1 or t1 or c1,t1 at the same time.

The amb8465 dongle (new model name is Metis-II) can listen to either
s1, c1 or t1.  It can also listen to c1 and t1 at the same time.

With the latest rtlwmbus you can listen to s1, c1 and t1 at
the same time. But you might want to disable some if you want
to save cpu usage.

The cul dongle can listen to c1 and t1 at the same time, but only
if you specify c1! If you specify t1 or s1, then it will only listen to t1 or s1.

Important!!!! Note that the cul dongle is limited to shorter
telegrams. There is a firmware fix that allows somewhat longer,
but still not full length telegrams. This can be a serious blocker
if you want to receive long telegrams from advanced meters.
Read the wiki to find this firmware.

The rc1180 dongle can listen only to t1.

# Usage examples

```shell
wmbusmeters auto:c1
```

Listens for C1 telegrams using any of your available wmbus dongles:
```
Received telegram from: 12345678
          manufacturer: (KAM) Kamstrup Energi (0x2c2d)
           device type: Cold water meter (0x16) encrypted
            device ver: 0x1b
                device: im871a[12345678]
                  rssi: -77 dBm
                driver: multical21
```

You can see that this telegram is encrypted and therefore you need a key.
Now listen to this specific meter, since the driver is auto-detected, we can use `auto` for the meter driver.

```shell
wmbusmeters auto:c1 MyTapWater auto 12345678 00112233445566778899AABBCCDDEEFF
```

(The Multical21 and other meters use compressed telegrams, which means
that you might have to wait up to 8 telegrams (8*16 seconds) until you
receive a full length telegram which gives all the information needed
to decode the compressed telegrams.)

Example output:

`MyTapWater   12345678     6.388 m3     6.377 m3    0.000 m3/h     8°C    23°C   DRY(dry 22-31 days)     2018-03-05 12:02.50`

(Here the multical21 itself, is configured to send target volume, therefore the max flow is 0.000 m3/h.)

Example format json output:

```shell
wmbusmeters --format=json /dev/ttyUSB0:im871a MyTapWater multical21:c1 12345678 00112233445566778899AABBCCDDEEFF MyHeater multical302 22222222 00112233445566778899AABBCCDDEEFF
```

```json
{"media":"cold water","meter":"multical21","name":"MyTapWater","id":"12345678","total_m3":6.388,"target_m3":6.377,"max_flow_m3h":0.000,"flow_temperature":8,"external_temperature":23,"current_status":"DRY","time_dry":"22-31 days","time_reversed":"","time_leaking":"","time_bursting":"","timestamp":"2018-02-08T09:07:22Z","device":"im871a[1234567]","rssi_dbm":-40}
```

```json
{"media":"heat","meter":"kamheat","name":"MyHeater","id":"22222222","total_kwh":0.000,"total_volume_m3":0.000,"current_kw":"0.000","timestamp":"2018-02-08T09:07:22Z"}
```

Example format fields output and use tuned rtlsdr dongle with rtlwmbus.

```shell
wmbusmeters --format=fields 'rtlwmbus(ppm=72)' GreenhouseWater multical21:c1 33333333 NOKEY
```

```
GreenhouseTapWater;33333333;9999.099;77.712;0.000;11;31;;2018-03-05 12:10.24
```

You can select a subset of all available fields, where we also select to print the timestamp as a unix timestamp.
The timestamp field is UTC time for json and local time when hr and fields. To explicitly select utc you
can specify timestamp_utc and timestamp_lt for local time.

```shell
wmbusmeters --format=fields --selectfields=id,total_m3,timestamp_ut,timestamp_utc /dev/ttyUSB0:im871a GreenhouseWater multical21:c1 33333333 NOKEY
```

```
33333333;9999.099;1628434800;2021-08-08T15:00.00Z
```

You can list all available fields for a meter: `wmbusmeters --listfields=multical21`

You can list all meters: `wmbusmeters --listmeters`

You can search for meters: `wmbusmeters --listmeters=water` or `wmbusmeters --listmeters=q`

Eaxmple of using the shell command to publish to MQTT:

```shell
wmbusmeters --shell='HOME=/home/you mosquitto_pub -h localhost -t water -m "$METER_JSON"' /dev/ttyUSB0:im871a GreenhouseWater multical21:c1 33333333 NOKEY
```

Example of using the shell command to inject data into postgresql database:

```shell
wmbusmeters --shell="psql waterreadings -c \"insert into readings values ('\$METER_ID',\$METER_TOTAL_M3,'\$METER_TIMESTAMP') \" " /dev/ttyUSB0:amb8465 MyColdWater multical21:c1 12345678 NOKEY
```

(It is much easier to add shell commands in the conf file since you do not need to quote the quotes.)

You can have multiple shell commands and they will be executed in the order you gave them on the command line.

To list the shell env variables available for a meter, run `wmbusmeters --listenvs=multical21` which outputs:

```
METER_JSON
METER_TYPE
METER_NAME
METER_ID
METER_TOTAL_M3
METER_TARGET_M3
METER_MAX_FLOW_M3H
METER_FLOW_TEMPERATURE_C
METER_EXTERNAL_TEMPERATURE_C
METER_CURRENT_STATUS
METER_TIME_DRY
METER_TIME_REVERSED
METER_TIME_LEAKING
METER_TIME_BURSTING
METER_TIMESTAMP
```

(If you have supplied `--field_floor=5` then you will also see `METER_floor` in the list)

Note that the `METER_TIMESTAMP` and the timestamp in the json output, is in UTC format, this is not your localtime.
However the hr and fields output will print your localtime.

You can add `shell=commandline` to a meter file stored in `wmbusmeters.d`, then this meter will use
this shell command instead of the command stored in `wmbusmeters.conf`.

You can use `--debug` to get both verbose output and the actual data bytes sent back and forth with the wmbus usb dongle.

If the meter does not use encryption of its meter data, then enter NOKEY on the command line.

```shell
wmbusmeters --format=json --meterfiles /dev/ttyUSB0:im871a:c1 MyTapWater multical21:c1 12345678 NOKEY
```

# Using wmbusmeters in a pipe

```shell
rtl_sdr -f 868.625M -s 1600000 - 2>/dev/null | rtl_wmbus -f -s | wmbusmeters --format=json stdin:rtlwmbus MyMeter auto 12345678 NOKEY | ...more processing...
```

Or you can send rtl_wmbus formatted telegrams using nc over UDP to wmbusmeters.
```shell
rtl_sdr -f 868.95M -s 1600000 - 2>/dev/null | rtl_wmbus -f -p s -a | nc -u localhost 4444
```

And receive the telegrams with nc spawned by wmbusmeters.
```shell
wmbusmeters 'rtlwmbus:CMD(nc -lku 4444)'
```

Or start nc explicitly in a pipe.
```shell
nc -lku 4444 | wmbusmeters stdin:rtlwmbus
```

Telegrams can also be pulled in by listening on MQTT topics if they were captured by other tools like [rtl_433](https://github.com/merbanan/rtl_433)
```shell
wmbusmeters 'hex:CMD(/usr/bin/mosquitto_sub -h 192.168.x.x -t rtl_433/device/devices/6/Wireless-MBus/+/data | tr -d "\n" )'
```
`+` is a wild card that listens to all the captured telegrams but can be replaced with a specific meter's ID

# Decoding hex string telegrams

If you have a single telegram as hex, which you want decoded, you do not need to create a simulation file,
you can just supply the telegram as a hex string on the command line.

```shell
wmbusmeters 234433300602010014007a8e0000002f2f0efd3a1147000000008e40fd3a341200000000
```

which will output:
```
No meters configured. Printing id:s of all telegrams heard!
Received telegram from: 00010206
          manufacturer: (LAS) Lansen Systems, Sweden (0x3033)
                  type: Other (0x00)
                   ver: 0x14
                driver: lansenpu
```

You can of course decode the meter on the fly:

```shell
wmbusmeters --format=json 234433300602010014007a8e0000002f2f0efd3a1147000000008e40fd3a341200000000 MyCounter auto 00010206 NOKEY
```

which will output:

```
{"media":"other","meter":"lansenpu","name":"MyCounter","id":"00010206","a_counter":4711,"b_counter":1234,"timestamp":"2021-09-12T08:45:52Z"}
```

You can also pipe the hex into wmbusmeters like this:

```shell
echo 234433300602010014007a8e0000002f2f0efd3a1147000000008e40fd3a341200000000 | ./build/wmbusmeters --silent --format=json stdin:hex MyCounter auto 00010206 NOKEY
```

or read hex data from a a file, `wmbusmeters myfile.txt:hex`

Any non-hex characters are ignored when using the suffix `:hex`. However when the hex string is
supplied on the command line it must be a proper hex string with no spaces.

When a telegram is supplied on the command line, then any valid dll crcs will be automatically removed,
like when the telegram is suppled in a simulation file.

You can analyze a telegram, this is useful when developing new drivers or trying
to find which driver is the best fit for an unknown mfct,type,ver combo.

```shell
wmbusmeters --analyze 3E44A5119035246141047A1A0030052F2F#0C06026301000C13688609040B3B0802000C2B220000F00A5A71020A5E72020AA61800004C0636370100426CBF25
```

Fields marked with `C!` (and green background) are content that is understood and put to use in the json. For example:
```
019 C!: 02630100 ("total_energy_consumption_kwh":16302)
```
Which shows the telegram raw data bytes and the json field into which the decoded value was presented.
Fields marked with `C?` (and red background) are content that is not understood nor used in the json.

To force a driver use: `--analyze=<driver>` to supply a decryption key: `--analyze=<key>` and to do both:
`--analyze=<key>:<driver>`


You can run the analyze functionality online here: [wmbusmeters.org](https://wmbusmeters.org)

# Additional tools

If you have a Kamstrup meter and you have received a KEM file and its
password from your supplier, then you can use `python2 utils/kem-import.py`
[utils/kem-import.py](utils/kem-import.py) to extract meter
information from that file (including the AES key) and to create
corresponding meter files in wmbusmeters' config directory.

You can also use the XMLExtract Java program. `javac utils/XMLExtract`
and then `java -cp utils XMLExtract` to print the key on the command line.

You can run wmbusmeters with `--logtelegrams` to get log output that can
be placed in a simulation.txt file. You can then run wmbusmeters and
instead of an usb device, you provide the `simulation.txt` file as
argument. See test.sh for more info.

If you do not specify any meters on the command line, then wmbusmeters
will listen and print the header information of any telegram it hears.

# Builds and runs on GNU/Linux MacOSX (with recent XCode), and FreeBSD

(For MacOSX do `brew install librtlsdr libusb` which takes such a long
time that the MacOSX travis build is disabled for the moment.)

`./configure && make && make test`

Binary generated: `./build/wmbusmeters`

`make install` will install this binary.

`make HOST=arm` to cross compile from GNU/Linux to Raspberry PI.

Binary generated: `./build_arm/wmbusmeters`

`make DEBUG=true`

Binary generated: `./build_debug/wmbusmeters`

`make testd` to run all tests using the debug build.

Debug builds only work on FreeBSD if the compiler is LLVM. If your
system default compiler is gcc, set `CXX=clang++` to the build
environment to force LLVM to be used.

`make DEBUG=true HOST=arm`

Binary generated: `./build_arm_debug/wmbusmeters`

# System configuration

`make install` installs the files:

`/etc/wmbusmeters.conf`
`/usr/bin/wmbusmeters`
`/usr/sbin/wmbusmetersd`
`/lib/systemd/system/wmbusmeters.service`
`/etc/logrotate.d/wmbusmeters`

creates these directories:

`/etc/wmbusmeters.d`
`/var/lib/wmbusmeters/meter_readings`

and adds the user `wmbusmeters` with no login account.

# Common problems

If wmbusmeters detects no device, but you know you have plugged in your wmbus dongle, then
run with `--verbose` to get more information on why the devices are not detected.
Typically this is because you are not in the dialout (for usb-serial dongles) or plugdev (for rtlsdr) group.

Run `sudo make install` to add the current user to the dialout group and the wmbusmeters group.

If the daemon has started then the wmbus device will be taken and you cannot start wmbusmeters manually.

To run manually, first make sure the daemon is stopped `sudo systemctl stop wmbusmeters`
if this hangs, then do `sudo killall -9 wmbusmetersd` and/or `sudo killall -9 wmbusmeters`.

## Non-standard baud rate set for AMB8465 USB stick

Wmbusmeters expects the serial baud rate for the AMB8465 USB stick to be 9600 8n1.
If you have used another tool and changed the baud rate to something else
you need to restore the baud rate to 9600 8n1.

If you like to send the bytes manually, the correct bytes are:

* Factory reset of the settings: `0xFF1100EE`
* Reset the stick to apply the factory defaults: `0xFF0500FA` this is not necessary if you unplug and reinsert the dongle.

# How to add a new driver

Drivers for OMS-compliant meters are text files `drivers/src/*.xmq`
First collect an unecrypted telegram as a hex string <hex> using --logtelegrams and any other driver.
Then run `wmbusmeters --analyze <hex>` to see the best match.

Copy that meters aaa,xmq file to a new filename bbb.xmq and change the name field from aaa to bbb in the driver source.

Now run the new driver with `wmbusmeters --analyze=drivers/src/bbb.xmq <hex>`
and start modifying the driver until it produces the desired json output.

You can now run `make; make install` from within the drivers directory
and then rebuild from the wmbusmeters directory `make`. The new driver is now
compiled into the binary.

You can also put the new driver file bbb.xmq into /etc/wmbusmeters.drivers.d and it will immediately
be available to the wmbusmeters program without recompiling.

# Caveat

If you do not get proper readings from the meters with non-standard protocols. apator162, mkradio3, vario451
then you have to open an issue here and help out by logging a lot of messages and reverse engineer them
even more..... :-/

# Good free documents on the wireless mbus protocol standard EN 13757

https://oms-group.org/

There is also a lot of wmbus protocol implementation details that
are missing. They will be added to the program as we figure out
how the meters send their data.
