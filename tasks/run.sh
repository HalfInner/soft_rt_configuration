#! /bin/sh
./task_server.a &
sleep 1
./task_client_a.a & 
./task_client_b.a & 

