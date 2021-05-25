from argparse import ArgumentParser
from Src.LightningsHistograms import *


class TimeHistogramMaker(HistogramMaker):
    def __init__(self):
        super().__init__()
        self.xlabel = 'мкс'


    def __get_data__(self):
        return list(map(lambda lightning: lightning.time, self.lightnings))


parser = ArgumentParser()
parser.add_argument("-l", "--lightning", dest="in_file_name",
                    help="input file with lightning coordinates", metavar="IN_FILE")
parser.add_argument("-c", "--columns", dest="number_columns", default=20, help="number columns in histogram")
parser.add_argument("-o", "--out", dest="out_file_name", default="out.png",
                    help="input file with lightning coordinates", metavar="OUT_FILE")
args = parser.parse_args()

histogram_maker = TimeHistogramMaker()
histogram_maker.make(args.in_file_name, args.out_file_name, args.number_columns)
