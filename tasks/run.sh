#! /usr/bin/bash
BIN_DIR=$1
./$BIN_DIR/task_server.a &
sleep 1
./$BIN_DIR/task_client.a "A" & 
./$BIN_DIR/task_client.a "B" & 

