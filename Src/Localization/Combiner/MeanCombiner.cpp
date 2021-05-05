#include "MeanCombiner.h"

namespace pl {
void MeanCombiner::MeanDataCombiner::add(double value) {
    mean_ = (mean_ * counter_ + value) / static_cast<double>(counter_ + 1);
    counter_++;
}

std::optional<double> MeanCombiner::MeanDataCombiner::get() {
    if (counter_ <= 0) {
        return std::nullopt;
    }

    return mean_;
}

bool MeanCombiner::MeanDataCombiner::hasValue() {
    return counter_ > 0;
}

void MeanCombiner::MeanDataCombiner::reset() {
    mean_ = 0;
    counter_ = 0;
}
}
