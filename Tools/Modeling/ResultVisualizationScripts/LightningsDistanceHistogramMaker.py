import math
from argparse import ArgumentParser
from Src.LightningsHistograms import *


class DistanceHistogramMaker(HistogramMaker):
    def __init__(self, signal):
        super().__init__()
        self.xlabel = 'км'
        self.signal = signal

    def __get_data__(self):
        return list(map(lambda lightning:
                        math.sqrt((lightning.x - self.signal.x) * (lightning.x - self.signal.x) +
                                  (lightning.y - self.signal.y) * (lightning.y - self.signal.y)),
                        self.lightnings))


parser = ArgumentParser()
parser.add_argument("-l", "--lightnings", dest="lightnings_file_name",
                    help="input file with lightnings coordinates", metavar="L_IN_FILE")
parser.add_argument("-s", "--signal", dest="signal_file_name",
                    help="input file with signal coordinates", metavar="S_IN_FILE")
parser.add_argument("-c", "--columns", dest="number_columns", default=15, help="number columns in histogram")
parser.add_argument("-o", "--out", dest="out_file_name", default="out.png",
                    help="input file with lightning coordinates", metavar="OUT_FILE")
args = parser.parse_args()

histogram_maker = DistanceHistogramMaker(read(args.signal_file_name)[0])
histogram_maker.make(args.lightnings_file_name, args.out_file_name, args.number_columns)
