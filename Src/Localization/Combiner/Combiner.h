#ifndef PLOC_COMBINER_H
#define PLOC_COMBINER_H

#include <optional>
#include <vector>

#include "Geometric/Points/TimePoint.h"

namespace pl {
/** Class for combining localization results */
class Combiner {
public:
    /**
     * Add new answer from locators
     *
     * @param new answer;
     * @param vector of locators by witch the answer was gotten.
     */
    virtual void add(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) = 0;

    /**
     * Count result of combining
     *
     * @return result of combining all added answers.
     */
    virtual std::optional<TimePoint<>> result() = 0;

    /** Reset class */
    virtual void reset() {};

    virtual ~Combiner() {}
};

}

#endif //PLOC_COMBINER_H
