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

        hist, bin_edges = np.histogram(data, bins=number_columns)
        plt.rc('axes', axisbelow=True)
        plt.xticks(bin_edges, rotation='vertical')
        plt.subplots_adjust(bottom=0.27)
        plt.xlabel(self.xlabel)
        plt.grid(axis='y')
        plt.hist(data, bins=bin_edges, color=[(69 / 255, 117 / 255, 180 / 255)], edgecolor='black', linewidth=1.2)
        plt.axvline(x=np.mean(data), color='red')
        plt.axvline(x=np.median(data), color='orange')
        plt.gcf().set_size_inches(9, 6.5)
        plt.subplots_adjust(top=0.96, right=0.96, left=0.075, bottom=0.175)
        plt.savefig(out_file_name)
