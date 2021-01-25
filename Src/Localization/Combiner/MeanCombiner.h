#ifndef PLOC_MEANCOMBINER_H
#define PLOC_MEANCOMBINER_H

#include "Combiner.h"

namespace pl {
class MeanCombiner: public Combiner {
public:
    void add(const TimePoint<>&) final;

    std::optional<TimePoint<>> result() final;

    void reset() final;

    ~MeanCombiner() {}

private:
    TimePoint<> mean_ = {0, 0, 0};
    TimePoint<unsigned int, unsigned int> numberOfAddedCoordinates_ = {0, 0, 0};
    bool isHasValue_ = false;

    static void updateMean(double& mean, double added, unsigned int& counter);
};

}

#endif //PLOC_MEANCOMBINER_H
