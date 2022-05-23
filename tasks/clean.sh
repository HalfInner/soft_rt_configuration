#! /usr/bin/bash
ps aux | grep task_ | tr -s ' ' | cut -f 2 -d ' ' | xargs kill -9

exit 0