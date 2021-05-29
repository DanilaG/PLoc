#ifndef PLOC_TIMEDIFFERENCECOMBINER_H
#define PLOC_TIMEDIFFERENCECOMBINER_H

#include "MassCombiner.h"

namespace pl {
class TimeDifferenceCombiner : public MassCombiner {
protected:
    double getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) final;
};

}

#endif //PLOC_TIMEDIFFERENCECOMBINER_H
