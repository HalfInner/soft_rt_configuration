#! /usr/bin/bash
# For more info: https://github.com/raspberrypi/linux/blob/rpi-3.2.27/Documentation/kernel-parameters.txt

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure|configure_regular> <dir> <shuffle_load>"
}

if [[ "$#" -eq 0 ]]; then
    echo "Not enough arguments 0"
    usage
    exit 1
fi

option=$1
dir=$2
shuffle_load=$3

defualt_isolated_cpu=3
if [[ "$option" == "test" ]]; then
    echo "Not Implemented"
elif [[ "$option" == "configure" ]]; then
    printf "Run on isolated core"
    output_file="/tmp/isolated_core.out"
    taskset -c $defualt_isolated_cpu ./$dir/task_server.a $shuffle_load &> $output_file &
    sleep 1
    taskset -c $defualt_isolated_cpu ./$dir/task_client_A.a "A" &
    sleep 1
    taskset -c $defualt_isolated_cpu ./$dir/task_client_B.a "B" &

    printf "Logs in file '$output_file'\n"
elif [[ "$option" == "configure_regular" ]]; then
    printf "Run on default core"
    output_file="/tmp/regular_core.out"
    ./$dir/task_server.a $shuffle_load  &> $output_file &
    sleep 1
    ./$dir/task_client_A.a "A" &
    sleep 1
    ./$dir/task_client_B.a "B" &

    printf "Logs in file '$output_file'\n"
else 
    usage
fi
