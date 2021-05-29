#include "TriangleCombiner.h"

namespace pl {
double TriangleCombiner::getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) {
    const int requiredNumberOfLocators = 3;
    assert(locators.size() == requiredNumberOfLocators && "TriangleCombiner work only with a triangle!");

    std::vector<double> sides(requiredNumberOfLocators);
    for (int i = 0; i < requiredNumberOfLocators; i++) {
        sides[i] = dist(locators[i], locators[(i + 1) % requiredNumberOfLocators]);
    }
    double minSide = *std::min_element(sides.begin(), sides.end());

    double p = (sides[0] + sides[1] + sides[2]) / 2.0;
    double S = sqrt(p * (p - sides[0]) * (p - sides[1]) * (p - sides[2]));

    double k = abs((sqrt(3) / 2) - ((2 * S) / (minSide * minSide)));
    return 1 / (k + 1) * S;
}

}
