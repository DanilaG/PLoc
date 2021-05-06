#ifndef PLOC_FILTEREDMEANCOMBINER_H
#define PLOC_FILTEREDMEANCOMBINER_H

#include "ParallelCombiner.h"

#include <vector>

namespace pl {
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

        const double left_percent_border = 0.1;
        const double right_percent_border = 0.1;
    private:
        std::vector<double> data_;
    };
};
}


#endif //PLOC_FILTEREDMEANCOMBINER_H
