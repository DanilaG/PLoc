#ifndef PLOC_FILTEREDMEANCOMBINER_H
#define PLOC_FILTEREDMEANCOMBINER_H

#include "ParallelCombiner.h"

#include <vector>

namespace pl {
/** Discarding extra values and calculate mean of others. */
class FilteredMeanCombiner: public ParallelCombiner {
public:
    FilteredMeanCombiner(): ParallelCombiner(std::make_shared<FilteredMeanDataCombiner>(),
                                             std::make_shared<FilteredMeanDataCombiner>(),
                                             std::make_shared<FilteredMeanDataCombiner>()) {}

protected:
    class FilteredMeanDataCombiner: public DataCombiner {
    public:
        void add(double) final;

        std::optional<double> get() final;

        bool hasValue() final;

        void reset() final;

        ~FilteredMeanDataCombiner() {}

        const double leftPercentBorderOfDiscarding = 0.1;
        const double rightPercentBorderOfDiscarding = 0.1;
        
    private:
        std::vector<double> data_;
    };
};

}


#endif //PLOC_FILTEREDMEANCOMBINER_H
