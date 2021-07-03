#ifndef PLOC_MEDIANCOMBINER_H
#define PLOC_MEDIANCOMBINER_H

#include "ParallelCombiner.h"

#include <vector>

namespace pl {
/** Calculates median value. */
class MedianCombiner: public ParallelCombiner {
public:
    MedianCombiner(): ParallelCombiner(std::make_shared<MedianDataCombiner>(),
                                     std::make_shared<MedianDataCombiner>(),
                                     std::make_shared<MedianDataCombiner>()) {}

protected:
    class MedianDataCombiner: public DataCombiner {
    public:
        void add(double) final;

        std::optional<double> get() final;

        bool hasValue() final;

        void reset() final;

        ~MedianDataCombiner() {}
    private:
        std::vector<double> data_;
    };
};

}

#endif //PLOC_MEDIANCOMBINER_H
