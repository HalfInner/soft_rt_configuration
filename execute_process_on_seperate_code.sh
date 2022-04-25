#! /bin/sh 
# For more info: https://github.com/raspberrypi/linux/blob/rpi-3.2.27/Documentation/kernel-parameters.txt

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure> <program>"
}

if [[ "$#" -eq 0 ]]; then
    echo "Not enough arguments 0"
    usage
    exit 1
fi

option=$1
program=$2
defualt_isolated_cpu=3
if [[ "$option" == "test" ]]; then
    echo "Not Implemented"
elif [[ "$option" == "configure" ]]; then
    taskset -c $defualt_isolated_cpu $program
else 
    usage
fi
