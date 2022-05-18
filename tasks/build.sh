#! /usr/bin/bash

SCRIPT_PATH=$(dirname "$0")
FLAGS='-Wall -Werror -Wpedantic -O3'
OUTPUT_DIR=$1

mkdir -p $OUTPUT_DIR

printf "Building task_server... "
g++ --std=c++2a $FLAGS -o $OUTPUT_DIR/task_server.a  $SCRIPT_PATH/task_print_server_high_prio.cc  -lrt -pthread &&\
printf "Done\n"

printf "Building task_client... "
g++ --std=c++2a $FLAGS -o $OUTPUT_DIR/task_client.a $SCRIPT_PATH/task_print_client_lower_prio.cc  -lrt &&\
printf "Done\n"

printf "Copy tasks... " 
cp $OUTPUT_DIR/task_client.a $OUTPUT_DIR/task_client_A.a
cp $OUTPUT_DIR/task_client.a $OUTPUT_DIR/task_client_B.a
printf "Done\n"

printf "Builds are in  '$OUTPUT_DIR'\n"
