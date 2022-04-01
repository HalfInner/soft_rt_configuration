#! /bin/sh

g++ --std=c++2a -O3 -o task_server.a   task_print_server_high_prio.cc  -lrt
g++ --std=c++2a -O3 -o task_client.a   task_print_client_a_lower_prio.cc  -lrt
