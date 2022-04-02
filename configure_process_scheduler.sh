#! /bin/sh 
# For more info: https://man7.org/linux/man-pages/man1/chrt.1.html

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure>"
}

option=$1
if [[ "$option" == "test" ]]; then
    project_task_pids=$( ps aux | egrep -v "(bash|grep)" |grep  task_|tr -s ' ' | cut -d ' ' -f 2)
    process_scheduler_list=$(echo $project_task_pids | xargs chrt -p )
    echo "$process_scheduler_list"
elif [[ "$option" == "configure" ]]; then
    echo "NotSupported"
else 
    usage
fi
