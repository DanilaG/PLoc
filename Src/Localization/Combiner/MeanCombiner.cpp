#include "MeanCombiner.h"

namespace pl {
void MeanCombiner::add(const TimePoint<>& newPoint) {
    updateMean(mean_.x, newPoint.x, numberOfAddedCoordinates_.x);
    updateMean(mean_.y, newPoint.y, numberOfAddedCoordinates_.y);
    updateMean(mean_.time, newPoint.time, numberOfAddedCoordinates_.time);
}

std::optional<TimePoint<>> MeanCombiner::result() {
    return numberOfAddedCoordinates_.x > 0 && numberOfAddedCoordinates_.y > 0 && numberOfAddedCoordinates_.time > 0 ?
           std::optional<TimePoint<>>(mean_) : std::nullopt;
}


void MeanCombiner::reset() {
    mean_ = {0, 0, 0};
    numberOfAddedCoordinates_ = {0, 0, 0};
}

void MeanCombiner::updateMean(double& mean, double added, unsigned int& counter) {
    if (!isnan(added)) {
        mean = (mean * counter + added) / static_cast<double>(counter + 1);
        counter++;
    }
}

}
