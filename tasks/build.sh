#! /usr/bin/bash

SCRIPT_PATH=$(dirname "$0")
FLAGS='-Wall -Werror -Wpedantic -O3'
OUTPUT_DIR="out"
mkdir -p $OUTPUT_DIR

printf "Building task_server... "
g++ --std=c++2a $FLAGS -o $OUTPUT_DIR/task_server.a  $SCRIPT_PATH/task_print_server_high_prio.cc  -lrt &&\
printf "Done\n"

printf "Building task_client_a... "
g++ --std=c++2a $FLAGS -o $OUTPUT_DIR/task_client_a.a $SCRIPT_PATH/task_print_client_a_lower_prio.cc  -lrt &&\
printf "Done\n"

printf "Building task_client_b... "
g++ --std=c++2a $FLAGS -o $OUTPUT_DIR/task_client_b.a $SCRIPT_PATH/task_print_client_b_lower_prio.cc  -lrt &&\
printf "Done\n" 

printf "Builds are in  '$OUTPUT_DIR'"
