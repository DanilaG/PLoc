#ifndef PLOC_FILTEREDMEANCOMBINER_H
#define PLOC_FILTEREDMEANCOMBINER_H

#include "Combiner.h"

#include <vector>

namespace pl {
class FilteredMeanCombiner: public Combiner {
public:
    void add(const TimePoint<>&) final;

    std::optional<TimePoint<>> result() final;

    void reset() final;

    ~FilteredMeanCombiner() {}

private:
    class FilteredData {
    public:
        void add(double);
        std::optional<double> get();
        void reset();

        bool hasValue();

        const double left_percent_border = 0.1;
        const double right_percent_border = 0.1;

    private:
        std::vector<double> data_;
    };
    FilteredData x_;
    FilteredData y_;
    FilteredData time_;
};
}


#endif //PLOC_FILTEREDMEANCOMBINER_H
