#include "TimeDifferenceCombiner.h"

namespace pl {
double TimeDifferenceCombiner::getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) {
    double mass = 0;
    for (unsigned int i = 0; i < (locators.size() - 1); i++) {
        for (unsigned int j = i + 1; j < locators.size(); j++) {
            mass += abs(locators[i].time - locators[j].time);
        }
    }
    return 1 / (mass + 1);
}
}
