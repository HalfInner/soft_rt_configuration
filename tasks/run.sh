#! /usr/bin/bash
BIN_DIR=$1
./$BIN_DIR/task_server.a &
sleep 1
./$BIN_DIR/task_client_a.a & 
./$BIN_DIR/task_client_b.a & 

