import copy
import re

class Lightning:
    def __init__(self, x_=0.0, y_=0.0, time_=0.0):
        self.x = x_
        self.y = y_
        self.time = time_


def mean(lightnings):
    number_elements = len(lightnings)
    result = Lightning()
    for lightning in lightnings:
        result.x += lightning.x
        result.y += lightning.y
        result.time += lightning.time
    result.x /= number_elements
    result.y /= number_elements
    result.time /= number_elements
    return result


def minMax(lightnings):
    min = copy.copy(lightnings[0])
    max = copy.copy(lightnings[0])
    for lightning in lightnings:
        if min.x > lightning.x:
            min.x = lightning.x
        if min.y > lightning.y:
            min.y = lightning.y
        if min.time > lightning.time:
            min.time = lightning.time
        if max.x < lightning.x:
            max.x = lightning.x
        if max.y < lightning.y:
            max.y = lightning.y
        if max.time < lightning.time:
            max.time = lightning.time
    return (min, max)


def read(file_pass):
    file = open(file_pass, "r")

    def LightningFromDescription(description):
        description = description.split(' ')
        return Lightning(float(description[0]),
                         float(description[1]),
                         float(description[2]) if len(description) > 2 else 0)

    data = re.sub(',? +', ' ', file.read()).split('\n')
    if data[0] != "\"x\" \"y\" \"time\"" and data[0] != "\"x\" \"y\"":
        exit("Wrong file format!")
    data.pop(0)
    data = list(filter(lambda str: str != '', data))
    result = list(map(lambda description: LightningFromDescription(description), data))

    file.close()
    return result
