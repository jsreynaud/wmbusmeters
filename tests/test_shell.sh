#!/bin/sh

PROG="$1"

mkdir -p testoutput
TEST=testoutput

TESTNAME="Test shell invocation"
TESTRESULT="ERROR"

$PROG --shell='echo "$METER_JSON"' simulations/simulation_shell.txt MWW supercom587 12345678 "" \
      2> $TEST/test_stderr.txt | jq --sort-keys . > $TEST/test_output.txt
if [ "$?" = "0" ]
then
    cat $TEST/test_output.txt | sed 's/"timestamp": "....-..-..T..:..:..Z"/"timestamp": "1111-11-11T11:11:11Z"/' > $TEST/test_responses.txt
    echo '{"_":"telegram","media":"warm water","meter":"supercom587","name":"MWW","id":"12345678","software_version":"010002","total_m3":5.548,"status":"OK","timestamp":"1111-11-11T11:11:11Z"}' \
         | jq --sort-keys . > $TEST/test_expected.txt
    diff $TEST/test_expected.txt $TEST/test_responses.txt
    if [ "$?" = "0" ]
    then
        echo OK: $TESTNAME
        TESTRESULT="OK"
    fi
fi

if [ "$TESTRESULT" = "ERROR" ]
then
    echo ERROR: $TESTNAME
    exit 1
fi
