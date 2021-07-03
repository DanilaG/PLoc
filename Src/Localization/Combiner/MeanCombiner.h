#ifndef PLOC_MEANCOMBINER_H
#define PLOC_MEANCOMBINER_H

#include "ParallelCombiner.h"

namespace pl {
/** Calculates mean value. */
class MeanCombiner: public ParallelCombiner {
public:
    MeanCombiner(): ParallelCombiner(std::make_shared<MeanDataCombiner>(),
                                     std::make_shared<MeanDataCombiner>(),
                                     std::make_shared<MeanDataCombiner>()) {}

protected:
    class MeanDataCombiner: public DataCombiner {
    public:
        void add(double) final;

        std::optional<double> get() final;

        bool hasValue() final;

        void reset() final;

        ~MeanDataCombiner() {}
    private:
        double mean_ = 0;
        unsigned int counter_ = 0;
    };
};

}

#endif //PLOC_MEANCOMBINER_H
