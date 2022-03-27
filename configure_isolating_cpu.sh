#! /bin/sh 
# For more info: https://github.com/raspberrypi/linux/blob/rpi-3.2.27/Documentation/kernel-parameters.txt

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure>"
}

option=$1
echo "$0::$1::$2::$3"
if [[ "$option" == "test" ]]; then
    current_config=$(cat /boot/cmdline.txt)
    echo "Current config: '$current_config'"

    current_isolated_cpu=$(cat /sys/devices/system/cpu/isolated)
    echo "Current isolated cpu: '$current_isolated_cpu'"
elif [[ "$option" == "configure" ]]; then
    user_who=$(whoami)
    if [[ "$user_who" == "root" ]]; then 
        arg_isolate_cpu="isolcpus=3"
        current_config=$(cat /boot/cmdline.txt)
        new_config="$current_config $arg_isolate_cpu"
        echo -n "$new_config" > /boot/cmdline.txt
        echo "Added isolated CPU"
        echo "Previous Config: $current_config"
        echo "     New Config: $new_config"
    else
        echo "Script has to be executed by the 'root' but was by '$user_who'"
    fi 
else 
    usage
fi
