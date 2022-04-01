#! /bin/sh
ps aux | grep task_server | tr -s ' ' | cut -f 2 -d ' ' | xargs kill -9
