#!/bin/bash

###############################################################################
# Run a single test generated by run_tests.py. Not for direct human use.
###############################################################################

test_dir=$(pwd)

test_id=$1
description=$2
yaml_file=$3
run_args=$4

cd .. # Navigate to folder with run.sh

mkdir -p $test_dir/pass_logs
mkdir -p $test_dir/fail_logs
mkdir -p $test_dir/stagnant_logs

PASS_LOG=$test_dir/pass_logs/$test_id.log # Log file location
FAIL_LOG=$test_dir/fail_logs/$test_id.log # Log file location
STAGNANT_LOG=$test_dir/stagnant_logs/$test_id.log # Log file location

failed=false
echo "LOG FILE for $test_id WITH PARAMETERS: $description." > $FAIL_LOG
if ! ./run.sh --input "$test_dir/$yaml_file" $run_args &>> "$FAIL_LOG"; then
    failed=true
fi

# Check for extra failure conditions, such as when using gdb:
if grep -q "Program received signal" "$FAIL_LOG"; then
    failed=true
fi

if $failed ; then
    echo -e "$test_id has FAILED.\nDescription: $description"
    exit 1 #Failure
elif grep -q "ANAMOLY: Stagnant network!" "$FAIL_LOG"; then
    echo "$test_id STAGNANT. Finished with nothing to do! This may be expected if many features are off. ($description)"
    mv $FAIL_LOG $STAGNANT_LOG
else
    echo "$test_id passed ($description)"
    mv $FAIL_LOG $PASS_LOG
fi
exit 0