#ifndef PLOC_PARALLELCOMBINER_H
#define PLOC_PARALLELCOMBINER_H

#include "Combiner.h"

#include <memory>

namespace pl {
/** Protocol a combiner with independent coordinates. */
class ParallelCombiner: public Combiner {
public:
    /** Protocol for single coordinate combiner. */
    class DataCombiner {
    public:
        /**
         * Add new value.
         *
         * @param new value.
         */
        virtual void add(double) = 0;

        /**
         * Get result.
         *
         * @return result of combining.
         */
        virtual std::optional<double> get() = 0;

        /**
         *
         * @return true if combiner has number value.
         */
        virtual bool hasValue() = 0;

        /** Reset combiner */
        virtual void reset() = 0;

        virtual ~DataCombiner() {}
    };


    ParallelCombiner(std::shared_ptr<ParallelCombiner::DataCombiner> x,
                     std::shared_ptr<ParallelCombiner::DataCombiner> y,
                     std::shared_ptr<ParallelCombiner::DataCombiner> time):
                     x_(x), y_(y), time_(time) {}

    void add(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) final;

    std::optional<TimePoint<>> result() final;

    void reset() final;

    ~ParallelCombiner() {}

private:
    std::shared_ptr<ParallelCombiner::DataCombiner> x_ = nullptr;
    std::shared_ptr<ParallelCombiner::DataCombiner> y_ = nullptr;
    std::shared_ptr<ParallelCombiner::DataCombiner> time_ = nullptr;
};

}

#endif //PLOC_PARALLELCOMBINER_H
