from argparse import ArgumentParser
import numpy as np
import matplotlib.pyplot as plt
import re

parser = ArgumentParser()
parser.add_argument("-l", "--lightning", dest="in_file_name",
                    help="input file with lightning coordinates", metavar="IN_FILE")
parser.add_argument("-c", "--columns", dest="number_columns", default=15, help="number columns in histogram")
parser.add_argument("-o", "--out", dest="out_file_name", default="out.png",
                    help="input file with lightning coordinates", metavar="OUT_FILE")
args = parser.parse_args()

f = open(args.in_file_name, "r")
number_input_parameters = 3
times = list(map(float, re.split('[\n ]', f.read())[2 * number_input_parameters - 1::3]))
f.close()

hist, bin_edges = np.histogram(times, bins=args.number_columns)
plt.rc('axes', axisbelow=True)
plt.xticks(bin_edges, rotation='vertical')
plt.subplots_adjust(bottom=0.27)
plt.xlabel('мкс')
plt.grid(axis='y')
plt.hist(times, bins=bin_edges, color=[(69 / 255, 117 / 255, 180 / 255)], edgecolor='black', linewidth=1.2)
plt.savefig(args.out_file_name)
