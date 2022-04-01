#! /bin/sh
ps aux | grep task_ | tr -s ' ' | cut -f 2 -d ' ' | xargs kill -9

