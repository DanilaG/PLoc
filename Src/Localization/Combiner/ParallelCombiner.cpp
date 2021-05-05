#include "ParallelCombiner.h"

namespace pl {
    void ParallelCombiner::add(const TimePoint<>& point) {
        x_->add(point.x);
        y_->add(point.y);
        time_->add(point.time);
    }

    std::optional<TimePoint<>> ParallelCombiner::result() {
        if (!x_->hasValue() || !y_->hasValue() || !time_->hasValue()) {
            return std::nullopt;
        }

        return TimePoint<>(x_->get().value(), y_->get().value(), time_->get().value());
    }

    void ParallelCombiner::reset() {
        x_->reset();
        y_->reset();
        time_->reset();
    }
}
