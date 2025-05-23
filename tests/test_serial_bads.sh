#!/bin/sh

PROG="$1"

mkdir -p testoutput

TEST=testoutput

####################################################
TESTNAME="Test ok serial rawtty telegram on stdin"
TESTRESULT="ERROR"

cat > $TEST/test_expected_unsorted.txt <<EOF
{"_":"telegram","media":"room sensor","meter":"lansenth","name":"Rummet1","id":"00010203","status":"PERMANENT_ERROR SABOTAGE_ENCLOSURE","current_temperature_c":21.8,"current_relative_humidity_rh":43,"average_temperature_1h_c":21.79,"average_relative_humidity_1h_rh":43,"average_temperature_24h_c":21.97,"average_relative_humidity_24h_rh":42.5,"timestamp":"1111-11-11T11:11:11Z"}
{"_":"telegram","media":"room sensor","meter":"rfmamb","name":"Rummet2","id":"11772288","status":"PERMANENT_ERROR","current_temperature_c":22.08,"average_temperature_1h_c":21.91,"average_temperature_24h_c":22.07,"maximum_temperature_1h_c":22.08,"maximum_temperature_24h_c":23.47,"minimum_temperature_1h_c":21.85,"minimum_temperature_24h_c":21.29,"current_relative_humidity_rh":44.2,"average_relative_humidity_1h_rh":43.2,"average_relative_humidity_24h_rh":44.5,"maximum_relative_humidity_1h_rh":44.2,"maximum_relative_humidity_24h_rh":50.1,"minimum_relative_humidity_1h_rh":42.5,"minimum_relative_humidity_24h_rh":42.2,"device_datetime":"2019-10-11 19:59","timestamp":"1111-11-11T11:11:11Z"}
EOF

jq --sort-keys . $TEST/test_expected_unsorted.txt > $TEST/test_expected.txt

xxd -r -p simulations/serial_rawtty_ok.hex | \
    $PROG --silent --format=json --listento=any stdin:rawtty \
          Rummet1 lansenth 00010203 "" \
          Rummet2 rfmamb 11772288 "" \
    | grep Rummet | jq --sort-keys . > $TEST/test_output.txt


if [ "$?" = "0" ]
then
    cat $TEST/test_output.txt | sed 's/"timestamp": "....-..-..T..:..:..Z"/"timestamp": "1111-11-11T11:11:11Z"/' > $TEST/test_responses.txt
    diff $TEST/test_expected.txt $TEST/test_responses.txt
    if [ "$?" = "0" ]
    then
        echo "OK: $TESTNAME"
        TESTRESULT="OK"
    else
        if [ "$USE_MELD" = "true" ]
        then
            meld $TEST/test_expected.txt $TEST/test_responses.txt
        fi
    fi
fi

if [ "$TESTRESULT" = "ERROR" ]; then echo ERROR: $TESTNAME;  exit 1; fi

####################################################
TESTNAME="Test bad serial rawtty telegram with interspersed noise, on stdin"
TESTRESULT="ERROR"

cat > $TEST/test_expected_unsorted.txt <<EOF
{"_":"telegram","media":"room sensor","meter":"rfmamb","name":"Rummet2","id":"11772288","status":"PERMANENT_ERROR","current_temperature_c":22.08,"average_temperature_1h_c":21.91,"average_temperature_24h_c":22.07,"maximum_temperature_1h_c":22.08,"maximum_temperature_24h_c":23.47,"minimum_temperature_1h_c":21.85,"minimum_temperature_24h_c":21.29,"current_relative_humidity_rh":44.2,"average_relative_humidity_1h_rh":43.2,"average_relative_humidity_24h_rh":44.5,"maximum_relative_humidity_1h_rh":44.2,"maximum_relative_humidity_24h_rh":50.1,"minimum_relative_humidity_1h_rh":42.5,"minimum_relative_humidity_24h_rh":42.2,"device_datetime":"2019-10-11 19:59","timestamp":"1111-11-11T11:11:11Z"}
EOF

jq --sort-keys . $TEST/test_expected_unsorted.txt > $TEST/test_expected.txt

xxd -r -p simulations/serial_rawtty_bad.hex | $PROG --silent --format=json --listento=any stdin:rawtty Rummet1 lansenth 00010203 "" Rummet2 rfmamb 11772288 "" | grep Rummet | jq --sort-keys . > $TEST/test_output.txt

if [ "$?" = "0" ]
then
    cat $TEST/test_output.txt | sed 's/"timestamp": "....-..-..T..:..:..Z"/"timestamp": "1111-11-11T11:11:11Z"/' > $TEST/test_responses.txt
    diff $TEST/test_expected.txt $TEST/test_responses.txt
    if [ "$?" = "0" ]
    then
        echo "OK: $TESTNAME"
        TESTRESULT="OK"
    else
        if [ "$USE_MELD" = "true" ]
        then
            meld $TEST/test_expected.txt $TEST/test_responses.txt
        fi
    fi
fi

if [ "$TESTRESULT" = "ERROR" ]; then echo ERROR: $TESTNAME;  exit 1; fi
