#! /bin/sh 
# For more info: man trace-cmd

server_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_server | tr -s ' ' | cut -d ' ' -f 2)
client_a_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_a | tr -s ' ' | cut -d ' ' -f 2)
client_b_task_pid=$(ps aux | egrep -v "(bash|grep)" | grep  task_client_b | tr -s ' ' | cut -d ' ' -f 2)
IDLE_pid=0

trace-cmd record  -P 11881 -P 11883 -P 11885 -P 0 -e sched:sched_wakeup -e sched:sched_switch &
tracer_pid=$!

sleep(5) # wait for a while to record traces

SIGINT=2
kill -$SIGINT $tracer_pid
trace-cmd report -t --ts-diff --cpu 3