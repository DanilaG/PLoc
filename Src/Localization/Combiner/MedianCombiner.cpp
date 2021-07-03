#include "MedianCombiner.h"

namespace pl {
void MedianCombiner::MedianDataCombiner::add(double value) {
    data_.push_back(value);
}

bool MedianCombiner::MedianDataCombiner::hasValue() {
    return !data_.empty();
}

std::optional<double> MedianCombiner::MedianDataCombiner::get() {
    if (!hasValue()) {
        return std::nullopt;
    }

    std::sort(data_.begin(), data_.end());
    return data_[data_.size() / 2];
}

void MedianCombiner::MedianDataCombiner::reset() {
    data_ = std::vector<double>();
}

}
