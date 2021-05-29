#ifndef PLOC_TIMESUMCOMBINER_H
#define PLOC_TIMESUMCOMBINER_H

#include "MassCombiner.h"

namespace pl {
class TimeSumCombiner : public MassCombiner {
protected:
    double getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) final;
};

}

#endif //PLOC_TIMESUMCOMBINER_H
