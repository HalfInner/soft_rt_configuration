#! /usr/bin/python3
from distutils.command.build_scripts import first_line_re
import sys

def conv_trace_result_from_file(file : str):
    timestamp_per_process = {}
    process_idx = 0
    timestamp_id = 2
    event_id = 4
    with open(file) as f:
        first_line = True
        second_line = False
        for line in f:
            if first_line:
                first_line = False
                second_line = True
                continue
            data = line.split()
            process = data[process_idx]
            timestamp = data[timestamp_id]
            event = event = data[event_id]
            if second_line:
                second_line = False
                event = data[event_id-1]
            if event != "sched_wakeup:":
                continue
            
            curr_timestamps = []
            if process in timestamp_per_process:
                curr_timestamps = timestamp_per_process[process]
            curr_timestamps.append(timestamp)
            timestamp_per_process[process] = curr_timestamps
            
    return timestamp_per_process
            
def main(argv):
    data = conv_trace_result_from_file(argv[1])
    for key, val in data.items():
        print(key, val)
    
if __name__ == "__main__":
    main(sys.argv)