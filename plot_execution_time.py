#! /usr/bin/python3
from csv import excel
from distutils.log import error
import math
from time import time
import matplotlib.pyplot as plt
import sys
from typing import List


class Task:
    def __init__(self, task_name):
        self.__task_name = task_name
        self.__axis_x = []
        self.__axis_y = []

    def name(self):
        return self.__task_name

    def add(self, timestamp, duration):
        self.__axis_x.append(timestamp)
        self.__axis_y.append(duration)

    def X(self):
        return self.__axis_x

    def Y(self):
        return self.__axis_y


def plot(tasks: List[Task], min_timestamp, max_timestamp, filename, store_filename):

    # _, axis = plt.subplots(nrows=channels, sharex='all', constrained_layout=False, figsize=(10,9))
    # for idx, chart in enumerate(process_charts):
    #     local_axis = axis[idx] if channels > 1 else axis
    #     local_axis.plot(chart.get_axis_x(), chart.get_axis_y())
    #     local_axis.set_title(chart.get_name(), loc='left', pad=.01)
    #     local_axis.set_xlim([min_timestamp, max_timestamp])
    plt.plot(tasks[0].X(), tasks[0].Y(),  'go--',
             linewidth=0.5, markersize=1, label=tasks[0].name())
    plt.plot(tasks[1].X(), tasks[1].Y(),  'ro--',
             linewidth=0.5, markersize=1, label=tasks[1].name())
    plt.plot(tasks[2].X(), tasks[2].Y(),  'bo--',
             linewidth=0.5, markersize=1, label=tasks[2].name())
    plt.xlabel('timestamp')
    plt.ylabel('t(us)')
    plt.title(filename.split('/')[-1])
    plt.ylim([0, 2500])
    plt.legend(loc='upper left')
    plt.tight_layout()
    if store_filename:
        plt.savefig(store_filename, dpi=300.)
    else:
        plt.show()


def conv_trace_result_from_file(filename):
    datei = {}
    min_timestamp = None
    max_timestamp = None
    with open(filename) as f:
        for line in f:
            data = line.split('::')
            if len(data) != 3:
                continue
            try:
                timestamp = int(data[0])
            except:
                continue
            if min_timestamp is None:
                min_timestamp = timestamp
                max_timestamp = timestamp
            min_timestamp = min(min_timestamp, timestamp)
            max_timestamp = max(max_timestamp, timestamp)

            task_name = data[1].strip()
            duration = int(data[2][0:-3])
            task = datei[task_name] if task_name in datei else Task(task_name)
            task.add(timestamp, duration)
            datei[task_name] = task

    return sorted(datei.values(), key=lambda task: task.name()), min_timestamp, max_timestamp


def main(argv):
    try:
        filename = argv[1]
        tasks, min_timestamp, max_timestamp = conv_trace_result_from_file(
            filename)
        store_filename = None
        if (len(argv) > 2):
            store_filename = argv[2]
        plot(tasks, min_timestamp, max_timestamp, filename, store_filename)
    except Exception as e:
        print("ERROR: Program crashed:", argv)
        print("ERROR:", e)
        raise e


if __name__ == "__main__":
    main(sys.argv)
