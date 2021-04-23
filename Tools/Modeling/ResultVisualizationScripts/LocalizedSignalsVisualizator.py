import matplotlib.pyplot as plt
import math
from argparse import ArgumentParser
from Src.Lightning import *
from matplotlib.patches import Ellipse


def variance_for_plane_coordinates(lightnings):
    v_x = 0
    v_y = 0
    for lightning in lightnings:
        delta = lightning.x - mean.x
        v_x += delta * delta
        delta = lightning.y - mean.y
        v_y += delta * delta
    v_x /= len(lightnings)
    v_y /= len(lightnings)
    return (v_x, v_y)


def correlation(lightnings, mean):
    k_numerator = 0
    k_first_denominator = 0
    k_second_denominator = 0
    for lightning in lightnings:
        delta_x = (lightning.x - mean.x)
        delta_y = (lightning.y - mean.y)
        k_numerator += delta_x * delta_y
        k_first_denominator += delta_x * delta_x
        k_second_denominator += delta_y * delta_y
    return k_numerator / (math.sqrt(k_first_denominator) * math.sqrt(k_second_denominator))


# Parser
parser = ArgumentParser()
parser.add_argument("-l", "--lightnings", dest="lightnings_file_name",
                    help="input file with lightnings coordinates", metavar="L_IN_FILE")
parser.add_argument("-s", "--signal", dest="signal_file_name",
                    help="input file with signal coordinates", metavar="S_IN_FILE")
parser.add_argument("-o", "--out", dest="out_file_name", default="out.png",
                    help="input file with lightning coordinates", metavar="OUT_FILE")
args = parser.parse_args()

# Algorithm
signal_source = read(args.signal_file_name)[0]
lightnings = read(args.lightnings_file_name)
for index in range(len(lightnings)):
    lightnings[index].x -= signal_source.x
    lightnings[index].y -= signal_source.y

mean = mean(lightnings)
v_x, v_y = variance_for_plane_coordinates(lightnings)
correlation = correlation(lightnings, mean)

alpha = 0.5 * math.atan((2 * correlation * math.sqrt(v_x) * math.sqrt(v_y)) / (v_x - v_y))
b = math.sqrt((v_x + v_y) * (v_x + v_y) - 4 * v_x * v_y * (1 - correlation * correlation))
k_a = math.sqrt(0.5 * (v_x + v_y + b))
k_b = math.sqrt(0.5 * (v_x + v_y - b))
c = 2 * math.log(1 / (1 - 0.9))

# Plotting
for lightning in lightnings:
    plt.plot(lightning.x, lightning.y, "o", color='black')
plt.plot(0.0, 0.0, "o", color='red', zorder=len(lightnings) + 1)

ellipse = Ellipse(xy=(mean.x, mean.y), width=k_b * c, height=k_a * c, angle=(alpha * 180.0 / math.pi),
                  color=(69 / 255, 117 / 255, 180 / 255), fc='None', lw=3, zorder=len(lightnings) + 2)
ax = plt.gca()
ax.add_patch(ellipse)

min_l, max_l = minMax(lightnings)
delta_x = abs(max_l.x - min_l.x)
delta_y = abs(max_l.y - min_l.y)
h = (delta_x if delta_x > delta_y else delta_y) * 1.1

ax.set_xlim(-h / 2, h / 2)
ax.set_ylim(-h / 2, h / 2)
plt.gcf().set_size_inches(6.5, 6.5)
plt.grid()
plt.xlabel('км')
plt.ylabel('км')
plt.subplots_adjust(top=0.96, right=0.96, left=0.125, bottom=0.125)
plt.savefig(args.out_file_name)
