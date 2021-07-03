#include "FilteredMeanCombiner.h"

#include <algorithm>
#include <cmath>

namespace pl {
void FilteredMeanCombiner::FilteredMeanDataCombiner::add(double value) {
    data_.push_back(value);
}

bool FilteredMeanCombiner::FilteredMeanDataCombiner::hasValue() {
    return !data_.empty();
}

std::optional<double> FilteredMeanCombiner::FilteredMeanDataCombiner::get() {
    if (!hasValue()) {
        return std::nullopt;
    }

    unsigned int leftBorder = 0;
    unsigned int rightBorder = data_.size();

    if (data_.size() > 3) {
        leftBorder += ceil(data_.size() * leftPercentBorderOfDiscarding);
        rightBorder -= ceil(data_.size() * rightPercentBorderOfDiscarding);
    }

    std::sort(data_.begin(), data_.end());
    double mean = 0;
    for (unsigned int i = leftBorder; i < rightBorder; i++) {
        mean += data_[i];
    }
    return mean / (rightBorder - leftBorder);
}

void FilteredMeanCombiner::FilteredMeanDataCombiner::reset() {
    data_ = std::vector<double>();
}

}
