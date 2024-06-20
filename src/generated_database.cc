/*
 Copyright (C) 2024 Fredrik Öhrström (gpl-3.0-or-later)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Generated 2024-06-20_10:13

BuiltinDriver builtins_[] =
{
    { "aquastream", "driver{name=aquastream meter_type=WaterMeter default_fields=name,id,total_m3,timestamp detect{mvt=IMT,01,07}use=total_m3,meter_datetime,target_m3,target_date field{name=status quantity=Text info=status_and_error_flags attributes=INCLUDE_TPL_STATUS match{measurement_type=Instantaneous vif_range=ErrorFlags}lookup{name=ERROR_FLAGS map_type=BitToString mask_bits=0xffff default_message=OK map{name=LOW_BATTERY info='Low Battery – 18 months before end of life'value=0x04 test=Set}map{name=TEMPORARY_ALARM info='Temporary alarm - Manipulation/module removed'value=0x10 test=Set}map{name=LEAKAGE info=Leakage value=0x20 test=Set}map{name=BURST info=Burst value=0x30 test=Set}}}field{name=total_tariff1 quantity=Volume match{measurement_type=Instantaneous vif_range=Volume tariff_nr=1}}field{name=battery quantity=Time display_unit=y force_scale=0.0027397260273972603 match{measurement_type=Instantaneous difvifkey=02FD74}}}", false },
    { "elster", "driver{name=elster meter_type=GasMeter default_fields=name,id,total_m3,timestamp detect{mvt=ELS,81,03}use=actuality_duration_s field{name=total quantity=Volume match{measurement_type=Instantaneous vif_range=Volume}about{de='Der Gesamtwasserverbrauch.'en='The total water consumption.'fr='''La consommation totale d'eau.'''sv='Den totala vattenförbrukningen.'}}}", false },
    { "eltako", "driver{name=eltako default_fields=name,id,total_kwh,timestamp meter_type=ElectricityMeter detect{mvt=ELT,01,02}field{name=status quantity=Text info=status_and_error_flags match{measurement_type=Instantaneous vif_range=ErrorFlags}lookup{name=ERROR_FLAGS map_type=BitToString mask_bits=0xff default_message=OK map{name=BUSY info='System is busy'value=0x01 test=Set}map{name=GENERIC_APP_ERROR info='Generic application error'value=0x02 test=Set}map{name=CURRENT_LOW info='Current too low'value=0x04 test=Set}map{name=PERMANENT_ERROR info='permanent error'value=0x08 test=Set}map{name=TEMPORARY_ERROR info='temporary error'value=0x10 test=Set}}}field{name=total_tariff_1 quantity=Energy info='Total cumulative active energy per tariff.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyEnergyVIF tariff_nr=1}}field{name=total_tariff_2 quantity=Energy info='Total cumulative active energy per tariff.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyEnergyVIF tariff_nr=2}}field{name='total_reactive_tariff_{tariff_counter}'quantity=Energy info='Total cumulative reactive energy per tariff.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyEnergyVIF storage_nr=2 tariff_nr=1,10}}field{name=voltage_l1_n quantity=Voltage info='Instantaneous voltage between L1 and neutral.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=Voltage add_combinable=Mfct01}}field{name=current_l1 quantity=Amperage info='Instantaneous current in the L1 phase.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=Amperage add_combinable=Mfct01}}field{name=power_l1 quantity=Power info='active Power in L1 phase'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF add_combinable=Mfct01}}field{name=reactive_power_l1 quantity=Power info='reactive Power in L1 phase'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF subunit_nr=1 add_combinable=Mfct01}}field{name=voltage_l2_n quantity=Voltage info='Instantaneous voltage between L2 and neutral.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=Voltage add_combinable=Mfct02}}field{name=current_l2 quantity=Amperage info='Instantaneous current in the L2 phase.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=Amperage add_combinable=Mfct02}}field{name=power_l2 quantity=Power info='active Power in L2 phase'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF add_combinable=Mfct02}}field{name=reactive_power_l2 quantity=Power info='reactive Power in L2 phase'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF subunit_nr=1 add_combinable=Mfct02}}field{name=voltage_l3_n quantity=Voltage info='Instantaneous voltage between L3 and neutral.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=Voltage add_combinable=Mfct03}}field{name=current_l3 quantity=Amperage info='Instantaneous current in the L3 phase.'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=Amperage add_combinable=Mfct03}}field{name=power_l3 quantity=Power info='active Power in L3 phase'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF add_combinable=Mfct03}}field{name=reactive_power_l3 quantity=Power info='reactive Power in L3 phase'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF subunit_nr=1 add_combinable=Mfct03}}field{name=ct_numerator info='Current transformer ratio (numerator)'quantity=Dimensionless vif_scaling=None match{difvifkey=02FF68}}field{name=active_power_overall quantity=Power info='active Power overall'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF add_combinable=Mfct00}}field{name=reactive_power_overall quantity=Power info='reactive Power overall'match{measurement_type=Instantaneous vif_scaling=Auto vif_range=AnyPowerVIF subunit_nr=1 add_combinable=Mfct00}}field{name=active_tariff info='active tariff'quantity=Dimensionless vif_scaling=None match{difvifkey=01FF13}}field{name=total quantity=Energy info='Total active energy over all tariffs.'calculate=total_tariff_1_kwh+total_tariff_2_kwh}}", false },
    { "ime", "driver{name=ime default_fields=name,id,total_kwh,timestamp meter_type=ElectricityMeter detect{mvt=IME,55,08}field{name=total quantity=Energy info='Positive Three-phase Active Energy (Total).'vif_scaling=None dif_signedness=Unsigned display_unit=kwh match{difvifkey=849010FF80843B}}}", false },
    { "iperl", "driver{name=iperl meter_type=WaterMeter default_fields=name,id,total_m3,max_flow_m3h,timestamp detect{mvt=SEN,68,06 mvt=SEN,68,07 mvt=SEN,7c,07}field{name=total quantity=Volume match{measurement_type=Instantaneous vif_range=Volume}about{de='Der Gesamtwasserverbrauch.'en='The total water consumption.'fr='''La consommation totale d'eau.'''sv='Den totala vattenförbrukningen.'}}field{name=max_flow quantity=Flow match{measurement_type=Instantaneous vif_range=VolumeFlow}about{en='The maximum flow recorded during previous period.'}}}", false },
    { "istaheat", "driver{name=istaheat meter_type=HeatMeter default_fields=name,id,status,total_kwh,target_kwh,timestamp detect{mvt=IST,a9,04}field{name=status quantity=Text info='Meter status from tpl field.'attributes=INCLUDE_TPL_STATUS}field{name=total quantity=Energy info='The total heat energy consumption recorded by this meter.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF}}field{name=target quantity=PointInTime info='Last day of previous billing month.'display_unit=date match{measurement_type=Instantaneous vif_range=Date storage_nr=2}}field{name=target quantity=Energy info='The total heat energy for the previous month period.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF storage_nr=2}}field{name=target quantity=Volume info='The total heating media volume recorded by this meter at the end of last month.'match{measurement_type=Instantaneous vif_range=Volume storage_nr=2}}field{name=last_year quantity=PointInTime info='Last day of previous billing year.'display_unit=date match{measurement_type=Instantaneous vif_range=Date storage_nr=1}}field{name=last_year quantity=Energy info='The total heat energy for the previous year period.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF storage_nr=1}}}", false },
    { "itronheat", "driver{name=itronheat meter_type=HeatMeter default_fields=name,id,status,total_kwh,timestamp detect{mvt=ITW,00,04}use=meter_datetime field{name=status quantity=Text info='Meter status from tpl field.'attributes=INCLUDE_TPL_STATUS}field{name=total quantity=Energy info='The total heat energy consumption recorded by this meter.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF}}field{name=total quantity=Volume info='The total volume of heating media recorded by this meter.'match{measurement_type=Instantaneous vif_range=Volume}}field{name=last_year quantity=PointInTime info='Last day of previous billing year.'display_unit=date match{measurement_type=Instantaneous vif_range=Date storage_nr=1}}field{name=last_year quantity=Energy info='The total heat energy for the previous year period.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF storage_nr=1}}}", false },
    { "kampress", "driver{name=kampress default_fields=name,id,status,pressure_bar,max_pressure_bar,min_pressure_bar,timestamp meter_type=PressureSensor detect{mvt=KAM,01,18}field{name=status quantity=Text info=status_and_error_flags match{measurement_type=Instantaneous vif_range=ErrorFlags}lookup{name=ERROR_FLAGS map_type=BitToString mask_bits=0xffff default_message=OK map{name=DROP info='Unexpected drop in pressure in relation to average pressure.'value=0x01 test=Set}map{name=SURGE info='Unexpected increase in pressure in relation to average pressure.'value=0x02 test=Set}map{name=HIGH info='Average pressure has reached configurable limit. Default 15 bar.'value=0x04 test=Set}map{name=LOW info='Average pressure has reached configurable limit. Default 1.5 bar.'value=0x08 test=Set}map{name=TRANSIENT info='Pressure changes quickly over short timeperiods. Average is fluctuating.'value=0x10 test=Set}map{name=COMM_ERROR info='Cannot measure properly or bad internal communication.'value=0x20 test=Set}}}field{name=pressure quantity=Pressure info='The measured pressure.'match{measurement_type=Instantaneous vif_range=Pressure}}field{name=max_pressure quantity=Pressure info='The maximum pressure measured during ?'match{measurement_type=Maximum vif_range=Pressure}}field{name=min_pressure quantity=Pressure info='The minimum pressure measured during ?'match{measurement_type=Minimum vif_range=Pressure}}field{name=alfa info='We do not know what this is.'quantity=Dimensionless vif_scaling=None match{difvifkey=05FF09}}field{name=beta info='We do not know what this is.'quantity=Dimensionless vif_scaling=None match{difvifkey=05FF0A}}}", false },
    { "sensostar", "driver{name=sensostar meter_type=HeatMeter default_fields=name,id,status,total_kwh,total_water_m3,target_date,target_kwh,timestamp detect{mvt=EFE,00,04}use=meter_datetime,fabrication_no,model_version,on_time_h,parameter_set field{name=status quantity=Text info='Meter status from tpl field.'attributes=INCLUDE_TPL_STATUS match{measurement_type=Instantaneous vif_range=ErrorFlags}lookup{name=ERROR_FLAGS map_type=BitToString mask_bits=0xff default_message=OK map{name=ERROR_TEMP_SENSOR_1_CABLE_BREAK value=0x01 test=Set}map{name=ERROR_TEMP_SENSOR_1_SHORT_CIRCUIT value=0x02 test=Set}map{name=ERROR_TEMP_SENSOR_2_CABLE_BREAK value=0x04 test=Set}map{name=ERROR_TEMP_SENSOR_2_SHORT_CIRCUIT value=0x08 test=Set}map{name=ERROR_FLOW_MEASUREMENT_SYSTEM_ERROR value=0x10 test=Set}map{name=ERROR_ELECTRONICS_DEFECT value=0x20 test=Set}map{name=OK_INSTRUMENT_RESET value=0x40 test=Set}map{name=OK_BATTERY_LOW value=0x80 test=Set}}}field{name=total quantity=Energy info='The total heat energy consumption recorded by this meter.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF}}field{name='total_tariff{tariff_counter}'quantity=Energy info='The total heat energy consumption recorded by this meter on tariff 2-3.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF tariff_nr=2,3}}field{name='total_subunit{subunit_counter}'quantity=Volume info='The total volume consumption recorded by this meter on subunit 1-3.'match{measurement_type=Instantaneous vif_range=Volume subunit_nr=1,3}}field{name='energy_consumption_semi_monthly_{storage_counter - 1 counter}'quantity=Energy match{measurement_type=Instantaneous vif_range=AnyEnergyVIF storage_nr=2,30}}field{name=total_water quantity=Volume info='The total volume of heating media recorded by this meter.'match{measurement_type=Instantaneous vif_range=Volume}}field{name=total_water_tariff2 quantity=Volume info='The total volume of heating media recorded by this meter on tariff2.'match{measurement_type=Instantaneous vif_range=Volume tariff_nr=2}}field{name=target_water quantity=Volume info='The total volume of heating media as recorded at the end of the billing period.'match{measurement_type=Instantaneous vif_range=Volume storage_nr=1}}field{name=power quantity=Power info='Power consumption.'match{measurement_type=Instantaneous vif_range=AnyPowerVIF}}field{name=power_max quantity=Power info='Power consumption.'match{measurement_type=Maximum vif_range=AnyPowerVIF}}field{name=flow_water quantity=Flow info='Flow of water.'match{measurement_type=Instantaneous vif_range=VolumeFlow}}field{name=flow_water_max quantity=Flow info='Flow of water maximum.'match{measurement_type=Maximum vif_range=VolumeFlow}}field{name=forward quantity=Temperature info='Forward temperature of water.'match{measurement_type=Instantaneous vif_range=FlowTemperature}}field{name=return quantity=Temperature info='Return temperature of water.'match{measurement_type=Instantaneous vif_range=ReturnTemperature}}field{name=difference quantity=Temperature info='Difference forward - return temperature.'match{measurement_type=Instantaneous vif_range=TemperatureDifference}}field{name=target quantity=PointInTime info='The reporting date of the last billing period.'display_unit=date match{measurement_type=Instantaneous vif_range=Date storage_nr=1}}field{name=target quantity=Energy info='The energy consumption at the last billing period date.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF storage_nr=1}}field{name='target_tariff{tariff_counter}'quantity=Energy info='The energy consumption at the last billing period date on tariff 2-3.'match{measurement_type=Instantaneous vif_range=AnyEnergyVIF storage_nr=1 tariff_nr=2,3}}field{name='target_subunit{subunit_counter}'quantity=Volume info='The volume consumption at the last billing period date on subunit 1-3.'match{measurement_type=Instantaneous vif_range=Volume storage_nr=1 subunit_nr=1,3}}}", false },
    { "supercal", "driver{name=supercal default_fields=name,id,total_kwh,timestamp meter_type=HeatMeter detect{mvt=SON,1b,04}use=meter_datetime,flow_temperature_c,return_temperature_c,volume_flow_m3h field{name=total quantity=Energy match{measurement_type=Instantaneous vif_range=AnyEnergyVIF}}field{name=total quantity=Volume match{measurement_type=Instantaneous vif_range=Volume}}field{name=target quantity=Energy match{measurement_type=Instantaneous vif_range=AnyEnergyVIF storage_nr=20}}field{name=target quantity=Volume match{measurement_type=Instantaneous vif_range=Volume storage_nr=20}}field{name=target quantity=PointInTime display_unit=date match{measurement_type=Instantaneous vif_range=Date storage_nr=20}}field{name=power quantity=Power match{measurement_type=Instantaneous vif_range=AnyPowerVIF}}}", false },
    { "werhlemodwm", "driver{name=werhlemodwm meter_type=WaterMeter default_fields=name,id,total_m3,timestamp detect{mvt=WZG,03,16}use=meter_datetime use=target_date use=target_m3 use=total_m3 use=fabrication_no field{name=next_target quantity=PointInTime display_unit=date match{measurement_type=Instantaneous vif_range=Date add_combinable=FutureValue storage_nr=1}}}", false },
};

MapToDriver builtins_mvts_[] =
{
    { { MANUFACTURER_IMT,0x01,0x07 }, "aquastream" },
    { { MANUFACTURER_ELS,0x81,0x03 }, "elster" },
    { { MANUFACTURER_ELT,0x01,0x02 }, "eltako" },
    { { MANUFACTURER_IME,0x55,0x08 }, "ime" },
    { { MANUFACTURER_SEN,0x68,0x06 }, "iperl" },
    { { MANUFACTURER_SEN,0x68,0x07 }, "iperl" },
    { { MANUFACTURER_SEN,0x7c,0x07 }, "iperl" },
    { { MANUFACTURER_IST,0xa9,0x04 }, "istaheat" },
    { { MANUFACTURER_ITW,0x00,0x04 }, "itronheat" },
    { { MANUFACTURER_KAM,0x01,0x18 }, "kampress" },
    { { MANUFACTURER_EFE,0x00,0x04 }, "sensostar" },
    { { MANUFACTURER_SON,0x1b,0x04 }, "supercal" },
    { { MANUFACTURER_WZG,0x03,0x16 }, "werhlemodwm" },
};
