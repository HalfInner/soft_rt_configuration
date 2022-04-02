#! /bin/sh 
# For more info: man trace-cmd

user_who=$(whoami)
if [[ "$user_who" != "root" ]]; then 
    echo "Script has to be executed by the 'root' but was by '$user_who'"
    exit 1
fi

server_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_server | tr -s ' ' | cut -d ' ' -f 2)
client_a_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_a | tr -s ' ' | cut -d ' ' -f 2)
client_b_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_b | tr -s ' ' | cut -d ' ' -f 2)
IDLE_pid=0

echo "Execute tracing"
trace-cmd record  \
    -P $server_task_pid -P $client_a_task_pid -P $client_b_task_pid -P $IDLE_pid \
    -e sched:sched_wakeup -e sched:sched_switch &
tracer_pid=$!

time_to_record=5
echo "Record for $time_to_record seconds"
sleep $time_to_record # record data for 5 seconds

SIGINT=2
kill -$SIGINT $tracer_pid
echo "Waits for a while to collect"
sleep 5 # wait for a while to collect traces

output_directory="~/soft_rt_configuration_logs/$(date +%s)"
mkdir -p $output_directory
trace-cmd report -t --ts-diff --cpu 3 > $output_directory/trace_report.txt
echo "Logs are under location: $output_directory"