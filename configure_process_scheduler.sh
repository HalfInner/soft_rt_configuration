#! /bin/sh 
# For more info: https://man7.org/linux/man-pages/man1/chrt.1.html


function usage() {
    echo "Usage:"
    echo "      $0 <test|configure>"
}

server_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_server | tr -s ' ' | cut -d ' ' -f 2)
client_a_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_a | tr -s ' ' | cut -d ' ' -f 2)
client_b_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_b | tr -s ' ' | cut -d ' ' -f 2)

option=$1
if [[ "$option" == "test" ]]; then
    echo $(chrt -p $server_task_pid)
    echo $(chrt -p $client_a_task_pid)
    echo $(chrt -p $client_b_task_pid)
elif [[ "$option" == "configure" ]]; then
    user_who=$(whoami)
    if [[ "$user_who" == "root" ]]; then 
        config='-f'
        echo $(chrt $config -p $server_task_pid)
        echo $(chrt $config -p $client_a_task_pid)
        echo $(chrt $config -p $client_b_task_pid)
    else
        echo "Script has to be executed by the 'root' but was by '$user_who'"
    fi 
else 
    usage
fi
