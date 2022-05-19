#! /usr/bin/bash 
# For more info: man trace-cmd

user_who=$(whoami)
if [[ "$user_who" != "root" ]]; then 
    echo "Script has to be executed by the 'root' but was by '$user_who'"
    exit 1
fi

tasks/clean.sh
build_output_directory="output_rpi4"
su pi -c "tasks/build.sh $build_output_directory"

LOAD_VALUE=35000
BASE_LOG_DIR="soft_rt_configuration_logs/"

function normal_execution_test() {
    server_output="/tmp/normal_execution.log"
    ./execute_process_on_seperate_code.sh configure_regular $build_output_directory $LOAD_VALUE $server_output

    ./execute_scheduler_tracer.sh false false
    ./execute_scheduler_tracer.sh false true
    ./execute_scheduler_tracer.sh true true
    tasks/clean.sh

    cp $server_output $BASE_LOG_DIR
}

function isolated_execution_test() {
    server_output="/tmp/isolated_execution.log"
    ./execute_process_on_seperate_code.sh configure $build_output_directory $LOAD_VALUE $server_output

    ./execute_scheduler_tracer.sh false false
    ./execute_scheduler_tracer.sh false true
    ./execute_scheduler_tracer.sh true true
    tasks/clean.sh

    cp $server_output $BASE_LOG_DIR
}

function isolated_fifo_execution_test() {
    server_output="/tmp/isolated_fifo_execution.log"
    ./execute_process_on_seperate_code.sh configure $build_output_directory $LOAD_VALUE $server_output

    ./execute_scheduler_tracer.sh false false
    ./execute_scheduler_tracer.sh false true
    ./execute_scheduler_tracer.sh true true
    tasks/clean.sh

    cp $server_output $BASE_LOG_DIR
}

normal_execution_test
isolated_execution_test
isolated_fifo_execution_test