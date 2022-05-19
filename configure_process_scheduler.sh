#! /usr/bin/bash 
# For more info: https://man7.org/linux/man-pages/man1/chrt.1.html


function usage() {
    echo "Usage:"
    echo "      $0 <test|configure> <reverse>"
}

server_task_name="task_server"
client_a_task_name="task_client_A"
client_b_task_name="task_client_B"

server_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  $server_task_name | tr -s ' ' | cut -d ' ' -f 2)
client_a_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  $client_a_task_name | tr -s ' ' | cut -d ' ' -f 2)
client_b_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  $client_b_task_name | tr -s ' ' | cut -d ' ' -f 2)

option=$1
reverse_mode=$2

if [[ "$option" == "test" ]]; then
    echo $(chrt -p $server_task_pid)
    echo $(chrt -p $client_a_task_pid)
    echo $(chrt -p $client_b_task_pid)
elif [[ "$option" == "configure" ]]; then
    user_who=$(whoami)
    if [[ "$user_who" == "root" ]]; then 
        config='-f'
        if [[ "$reverse_mode" == "true" ]]; then
            echo "Reverse Prio on"
            echo $server_task_name   $(chrt $config -p 97 $server_task_pid)
            echo $client_a_task_name $(chrt $config -p 98 $client_a_task_pid)
            echo $client_b_task_name $(chrt $config -p 99 $client_b_task_pid)
        else
            echo $server_task_name   $(chrt $config -p 99 $server_task_pid)
            echo $client_a_task_name $(chrt $config -p 98 $client_a_task_pid)
            echo $client_b_task_name $(chrt $config -p 97 $client_b_task_pid)
        fi
    else
        echo "Script has to be executed by the 'root' but was by '$user_who'"
    fi 
else 
    usage
fi
