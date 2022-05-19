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

