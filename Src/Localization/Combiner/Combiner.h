#ifndef PLOC_COMBINER_H
#define PLOC_COMBINER_H

#include <optional>
#include <vector>

#include "Geometric/Points/TimePoint.h"

namespace pl {
/** Class for combining localization results */
class Combiner {
public:
    /** Add new answer from locators */
    virtual void add(const TimePoint<>& answer, const std::vector<Point<>>& locators) = 0;
    /** Count result of combining */
    virtual std::optional<TimePoint<>> result() = 0;
    /** Reset class */
    virtual void reset() {};

    virtual ~Combiner() {}
};

}

#endif //PLOC_COMBINER_H
