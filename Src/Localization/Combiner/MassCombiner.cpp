#include "MassCombiner.h"

namespace pl {
    void MassCombiner::add(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) {
        data_.emplace_back(answer, getMass(answer, locators));
    }

    std::optional<TimePoint<>> MassCombiner::result() {
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

    void MassCombiner::reset() {
        data_.clear();
    }
}