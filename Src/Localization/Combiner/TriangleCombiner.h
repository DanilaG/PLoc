#ifndef PLOC_TRIANGLECOMBINER_H
#define PLOC_TRIANGLECOMBINER_H

#include <utility>
#include <vector>

#include "MassCombiner.h"

namespace pl {
class TriangleCombiner: public MassCombiner {
protected:
    double getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) final;
};

}

#endif //PLOC_TRIANGLECOMBINER_H
