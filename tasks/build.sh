#! /bin/sh
FLAGS='-Wall -Werror -Wpedantic -O3'
printf "Building task_server... "
g++ --std=c++2a $FLAGS -o task_server.a   task_print_server_high_prio.cc  -lrt
printf "Done\n"

printf "Building task_client_a... "
g++ --std=c++2a $FLAGS -o task_client_a.a   task_print_client_a_lower_prio.cc  -lrt
printf "Done\n"

printf "Building task_client_b... "
g++ --std=c++2a $FLAGS -o task_client_b.a   task_print_client_b_lower_prio.cc  -lrt
printf "Done\n"

echo $PWD