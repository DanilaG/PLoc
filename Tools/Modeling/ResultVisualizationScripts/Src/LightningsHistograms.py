import numpy as np
import matplotlib.pyplot as plt
from abc import abstractmethod
from Src.Lightning import *


class HistogramMaker:
    def __init__(self):
        self.lightnings = []
        self.xlabel = ''

    @abstractmethod
    def __get_data__(self):
        ...

    def make(self, in_file_name, out_file_name, number_columns):
        self.lightnings = read(in_file_name)

        data = self.__get_data__()

        max_value = max(data)
        min_value = min(data)

        step = (max_value - min_value) / number_columns
        if min_value >= 0:
            min_value = 0
        else:
            m = 0
            max_border = min_value - 0.5 * step
            start = min_value
            while abs(start) < 1:
                m += 1
                start *= 10
                max_border *= 10
            while abs(start) >= 10:
                m -= 1
                start /= 10
                max_border /= 10
            while abs(int(start - 1)) > abs(max_border):
                m += 1
                start *= 10
                max_border *= 10
            to_half = 5 - (abs(int(start)) % 10)
            if to_half > 0 and abs(int(start - to_half)) <= max_border:
                min_value = int(start - to_half) / pow(10, m)
            else:
                min_value = int(start - 1) / pow(10, m)
            step = (max_value - min_value) / number_columns
        if step != 0:
            m = 0
            max_step_offset = (1 + 1 / (number_columns)) * step
            rounded_step = step
            while abs(rounded_step) < 1:
                m += 1
                rounded_step *= 10
                max_step_offset *= 10
            while abs(rounded_step) >= 10:
                m -= 1
                rounded_step /= 10
                max_step_offset /= 10
            while int(rounded_step + 1) > max_step_offset:
                m += 1
                rounded_step *= 10
                max_step_offset *= 10
            to_half = 5 - (int(rounded_step) % 10)
            if to_half > 0 and int(rounded_step + to_half) <= max_step_offset:
                step = int(rounded_step + to_half) / pow(10, m)
            else:
                step = int(rounded_step + 1) / pow(10, m)

        bin_edges = []
        for i in range(number_columns + 1):
            bin_edges.append(min_value + step * i)

        plt.rcParams.update({'font.size': 12})
        plt.rc('axes', axisbelow=True)
        plt.xticks(bin_edges, rotation='vertical')
        plt.subplots_adjust(bottom=0.27)
        plt.xlabel(self.xlabel)
        plt.grid(axis='y')
        plt.hist(data, bins=bin_edges, color=[(69 / 255, 117 / 255, 180 / 255)], edgecolor='black', linewidth=1.2)
        plt.axvline(x=np.mean(data), color='red')
        plt.axvline(x=np.median(data), color='orange')
        plt.xlim(bin_edges[0], bin_edges[-1])
        plt.gcf().set_size_inches(8, 5)
        plt.subplots_adjust(top=0.96, right=0.96, left=0.075, bottom=0.25)
        plt.savefig(out_file_name)
