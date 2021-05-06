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

        unsigned int left_border = 0;
        unsigned int right_border = data_.size();

        if (data_.size() > 3) {
            left_border += ceil(data_.size() * left_percent_border);
            right_border -= ceil(data_.size() * right_percent_border);
        }

        std::sort(data_.begin(), data_.end());
        double mean = 0;
        for (unsigned int i = left_border; i < right_border; i++) {
            mean += data_[i];
        }
        return mean / (right_border - left_border);
    }

    void FilteredMeanCombiner::FilteredMeanDataCombiner::reset() {
        data_ = std::vector<double>();
    }
}
