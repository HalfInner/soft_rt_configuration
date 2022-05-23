#! /usr/bin/bash
BIN_DIR=$1
SHUFFLE_LOAD=$2
echo
./$BIN_DIR/task_server.a $SHUFFLE_LOAD &
sleep 1
echo 
./$BIN_DIR/task_client_A.a "A" & 
echo
./$BIN_DIR/task_client_B.a "B" & 

