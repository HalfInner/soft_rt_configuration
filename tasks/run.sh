#! /usr/bin/bash
BIN_DIR=$1
echo
./$BIN_DIR/task_server.a &
sleep 1
echo 
./$BIN_DIR/task_client_A.a "A" & 
echo
./$BIN_DIR/task_client_B.a "B" & 

