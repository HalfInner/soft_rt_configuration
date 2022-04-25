#! /bin/sh 
# For more info: https://github.com/raspberrypi/linux/blob/rpi-3.2.27/Documentation/kernel-parameters.txt

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure>"
}

option=$1
if [[ "$option" == "test" ]]; then
    current_affinity=$(sudo ps -ae -o psr= -o pid= -o command= | sort -k 1)
    echo "$current_affinity"
elif [[ "$option" == "configure" ]]; then
    echo "NotSupported"
else 
    usage
fi
