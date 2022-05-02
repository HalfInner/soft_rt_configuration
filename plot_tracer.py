#! /usr/bin/python3
import math
import matplotlib.pyplot as plt
import sys


def conv_trace_result_from_file(file: str):
    timestamp_per_process = {}
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
            timestamp = float(timestamp.replace(':', ''))
            curr_timestamps.append(timestamp)
            min_timestamp = min(timestamp, min_timestamp)
            max_timestamp = max(timestamp, max_timestamp)
            timestamp_per_process[process] = curr_timestamps

    return timestamp_per_process, min_timestamp, max_timestamp


class ProcessChart:
    IDX = 0
    def __init__(self, name: str):
        self.__y_offset = ProcessChart.IDX
        # ProcessChart.IDX += 1
        self.__process_name = name
        self.__axis_x = [0]
        self.__axis_y = [self.__y_offset]

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
        epsilon = .000000001
        self.__axis_y.append(self.__axis_y[-1])
        self.__axis_x.append(timestamp - epsilon)
        if self.__process_name == "<idle>-0":
            print(timestamp, self.__axis_x[-1])

        self.__axis_y.append(self.__y_offset + int(not to_zero))
        self.__axis_x.append(timestamp)
        if self.__process_name == "<idle>-0":
            print(timestamp, self.__axis_x[-1])

def conv_trace_from_set_to_data_plot(data):
    process_charts = []
    process_indicies = []
    keys = list(data.keys())
    for key in keys:
        process_charts.append(ProcessChart(key))
        process_indicies.append(0)

    any_process_not_empty = True
    while (any_process_not_empty):
        any_process_not_empty = False
        key_idx = 0
        min_float = 0xffffffffffffffffffffffff
        min_key_idx = -1
        for key, val in data.items():
            idx = process_indicies[key_idx]
            chart = process_charts[key_idx]
            if key != chart.get_name():
                raise "Key does not match chart"
            if idx < len(val):
                any_process_not_empty = True
                timestamp = val[idx]
                if timestamp < min_float:
                    min_float = timestamp
                    min_key_idx = key_idx
            key_idx += 1

        for idx, process in enumerate(process_charts):
            if idx == min_key_idx:
                process.turn_on(min_float)
                process_indicies[min_key_idx] += 1
            else:
                process.turn_off(min_float)
    return process_charts


def plot_set(process_charts, min_timestamp, max_timestamp):
    fig, axis = plt.subplots(nrows=len(process_charts))
    for idx, chart in enumerate(process_charts):
        axis[idx].plot(chart.get_axis_x(), chart.get_axis_y())
        # axis[idx].
        axis[idx].set_title(chart.get_name())
        print(chart.get_name(), '\n', chart.get_axis_x(), '\n', chart.get_axis_y())
    plt.xlim([min_timestamp, max_timestamp])
    plt.legend()
    plt.show()


def main(argv):
    data, min_timestamp, max_timestamp = conv_trace_result_from_file(argv[1])
    process_chart = conv_trace_from_set_to_data_plot(data)
    plot_set(process_chart, min_timestamp, max_timestamp)


if __name__ == "__main__":
    main(sys.argv)
