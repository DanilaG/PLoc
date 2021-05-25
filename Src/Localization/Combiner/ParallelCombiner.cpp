#include "ParallelCombiner.h"

namespace pl {
    void ParallelCombiner::add(const TimePoint<>& answer, const std::vector<Point<>>& locators) {
        x_->add(answer.x);
        y_->add(answer.y);
        time_->add(answer.time);
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
