#! /usr/bin/bash 
# For more info: man trace-cmd

user_who=$(whoami)
if [[ "$user_who" != "root" ]]; then 
    echo "Script has to be executed by the 'root' but was by '$user_who'"
    exit 1
fi

only_main_processes="false"
include_stress="false"
if [[ "$#" -ge  "2" ]]; then
    only_main_processes=$1
fi
if [[ "$#" -ge  "3" ]]; then
    include_stress=$2
fi
echo "Execute tracer with parameters only_main_processes='$1' include_stress='$2'"

server_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_server | tr -s ' ' | cut -d ' ' -f 2)
client_a_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_a | tr -s ' ' | cut -d ' ' -f 2)
client_b_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_b | tr -s ' ' | cut -d ' ' -f 2)
IDLE_pid=0

args=
if [[  "$only_main_processes" -eq "true" ]]; then
    args="-P $server_task_pid -P $client_a_task_pid -P $client_b_task_pid -P $IDLE_pid"
fi

stress_pid=
if [[ "$include_stress" -eq "true" ]]; then
    echo "Run stress CPU"
    su pi -c "stress -c 4 -t 25 > /dev/null &"
    stress_pid=$!
fi

echo "Execute tracing"
trace-cmd record  \
    -e sched:sched_wakeup -e sched:sched_switch $args &
tracer_pid=$!

time_to_record=10
echo "Record for $time_to_record seconds"
sleep $time_to_record # record data for 5 seconds

SIGINT=2
kill -$SIGINT $tracer_pid
# if [[ "$include_stress" -eq "true" ]]; then
#     echo "Stop "
#     kill -$SIGINT $stress_pid
# fi

echo "Waits for a while to collect"
sleep 5 # wait for a while to collect traces


output_directory="soft_rt_configuration_logs/$(date +%d_%m_%y__%H_%M_%S)"
mkdir -p $output_directory
trace-cmd report -t --ts-diff --cpu 3 > $output_directory/trace_report_cpu3.txt
trace-cmd report -t --ts-diff --cpu 2 > $output_directory/trace_report_cpu2.txt
trace-cmd report -t --ts-diff --cpu 1 > $output_directory/trace_report_cpu1.txt
trace-cmd report -t --ts-diff --cpu 0 > $output_directory/trace_report_cpu0.txt
trace-cmd report -t --ts-diff > $output_directory/trace_report_full.txt

chown pi $output_directory
chown pi $output_directory/trace_report*.txt
echo "Logs are under location: $output_directory"