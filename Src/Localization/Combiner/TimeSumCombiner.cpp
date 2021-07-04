#include "TimeSumCombiner.h"

namespace pl {
double TimeSumCombiner::getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) {
    double mass = 0;
    for (unsigned int i = 0; i < (locators.size()); i++) {
        mass += locators[i].time;
    }
    mass = mass * mass * mass * mass * mass * mass * mass * mass * mass * mass;
    return 1 / (mass + 1);
}

}
