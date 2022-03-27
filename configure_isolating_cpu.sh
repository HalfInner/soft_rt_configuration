#! bin/sh 
# For more info: https://github.com/raspberrypi/linux/blob/rpi-3.2.27/Documentation/kernel-parameters.txt

function usage() {
    echo "Usage:"
    echo "      $0 <test|confiure>"
}

option=$1
if [[ "$opition" -eq "test" ]]; then
    current_config=$(cat /boot/cmdline.txt)
    echo "Current config: '$current_config'"

    current_isolated_cpu=$(cat /sys/devices/system/cpu/isolated)
    echo "Current isolated cpu: '$current_isolated_cpu'"
elif [[ "$opition" -eq "configure" ]]; then
    user_who=$(whoami)
    if [[ "user_who" -eq "root" ]]; then 
        arg_isolate_cpu="isolcpus=3"
        echo "$arg_isolate_cpu" >> /boot/cmdline.txt
        echo "Added isolated CPU"
    else
        echo "Script has to be executed by the 'root' but was by '$user_who'"
    fi 
else 
    usage()
fi
