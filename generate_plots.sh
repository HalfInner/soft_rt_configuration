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

LOG_DIRECTORY=$1
for f in $LOG_DIRECTORY/*.log ; do
    echo "Plot '$f'"
    python plot_execution_time.py $f $f.png &
done

for d in $LOG_DIRECTORY/*/ ; do
    echo "$d"
    for subd in $d/*.txt; do
        echo "Plot $subd"
        python plot_tracer.py $subd $subd.png &
    done
done

