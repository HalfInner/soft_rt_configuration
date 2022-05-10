#! /usr/bin/python3
import math
import matplotlib.pyplot as plt
import sys


class ProcessChart:
    def __init__(self, name: str):
        self.__process_name = name
        self.__axis_x = [0]
        self.__axis_y = [0]

    def turn_on(self, timestamp: float):
        self.__activate(False, timestamp)

    def turn_off(self, timestamp: float):
        self.__activate(True, timestamp)

    def get_axis_x(self):
        return self.__axis_x

    def get_axis_y(self):
        return self.__axis_y

    def get_name(self):
        return self.__process_name

    def __activate(self, to_zero: bool, timestamp: float):
        epsilon = .0000000000000001
        epsilon = 0.0
        self.__axis_y.append(self.__axis_y[-1])
        self.__axis_x.append(timestamp - epsilon)

        self.__axis_y.append(int(not to_zero))
        self.__axis_x.append(timestamp)


def conv_trace_result_from_file(file: str):
    processes = {}
    process_idx = 0
    timestamp_id = 2
    event_id = 4
    min_timestamp = 0xffffffffffffffffffffffffffffffffffff
    max_timestamp = -1
    with open(file) as f:
        first_line = True
        second_line = False
        for line in f:
            if first_line:
                first_line = False
                second_line = True
                continue
            data = line.split()
            process = data[process_idx].split('-')[0]
            timestamp = float(data[timestamp_id].replace(':', ''))
            min_timestamp = min(timestamp, min_timestamp)
            max_timestamp = max(timestamp, max_timestamp)
            event = event = data[event_id]
            if second_line:
                second_line = False
                event = data[event_id-1]

            if event == "sched_wakeup:":
                curr_process = processes[process] if process in processes else ProcessChart(
                    process)
                curr_process.turn_on(timestamp)
                for process_ in processes.values():
                    if process_.get_name() != curr_process.get_name():
                        process_.turn_off(timestamp)
                processes[process] = curr_process

            if event == "sched_switch:":  # and process == "<idle>":
                next_task = data[-2].split(':')[0]
                if next_task.split('/')[0] == "swapper":
                    next_task = "<idle>"
                if next_task != "<idle>" and "task_" not in next_task:
                    next_task = "other"
                curr_process = processes[next_task] if next_task in processes else ProcessChart(
                    next_task)
                curr_process.turn_on(timestamp)
                for process_ in processes.values():
                    if process_.get_name() != curr_process.get_name():
                        process_.turn_off(timestamp)
                processes[next_task] = curr_process

    return processes.values(), min_timestamp, max_timestamp


def plot_set(process_charts, min_timestamp, max_timestamp, store_filename):
    _, axis = plt.subplots(nrows=len(process_charts), sharex='all')
    for idx, chart in enumerate(process_charts):
        axis[idx].plot(chart.get_axis_x(), chart.get_axis_y())
        axis[idx].set_title(chart.get_name())
        axis[idx].set_xlim([min_timestamp, max_timestamp])
        # axis[idx].set_xlim([min_timestamp, min_timestamp+1 ])
    if store_filename:
        plt.savefig(store_filename)
    else:
        plt.show()


def main(argv):
    processes, min_timestamp, max_timestamp = conv_trace_result_from_file(
        argv[1])
    store_filename=None
    if (len(argv) > 2):
        store_filename = argv[2]
    plot_set(processes, min_timestamp, max_timestamp, store_filename)


if __name__ == "__main__":
    main(sys.argv)
