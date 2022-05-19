#! /usr/bin/bash 
# For more info: https://github.com/raspberrypi/linux/blob/rpi-3.2.27/Documentation/kernel-parameters.txt

function usage() {
    echo "Usage:"
    echo "      $0 <directory>"
}

if [[ "$#" -ne "1" ]]; then
    echo "Not enough number of paramaters '$#'"
    usage
    exit 1
fi

directory=$1
for d in $directory/*/ ; do
    echo "$d"
    for subd in $d/*.txt; do
        echo "Plot $subd"
        python plot_tracer.py $subd $subd.png &
    done
done
