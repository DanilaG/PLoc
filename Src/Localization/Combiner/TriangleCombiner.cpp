#include "TriangleCombiner.h"

namespace pl {
void TriangleCombiner::add(const TimePoint<>& answer, const std::vector<Point<>>& locators) {
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
    double mass = 1 / (k + 1) * S;

    data_.emplace_back(answer, mass);
}

std::optional<TimePoint<>> TriangleCombiner::result() {
    if (data_.empty()) {
        return std::nullopt;
    }

    TimePoint<> result = {0, 0, 0};
    double fullMass = 0;
    for (auto& i: data_) {
        result.x += i.second * i.first.x;
        result.y += i.second * i.first.y;
        result.time += i.second * i.first.time;
        fullMass += i.second;
    }
    return TimePoint<>{result.x / fullMass, result.y / fullMass, result.time / fullMass};
}

void TriangleCombiner::reset() {
    data_.clear();
}
}
