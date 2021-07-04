#ifndef PLOC_MASSCOMBINER_H
#define PLOC_MASSCOMBINER_H

#include "Combiner.h"

#include <utility>
#include <vector>

namespace pl {
/** Combines result by the mass */
class MassCombiner : public Combiner {
public:
    void add(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) final;
    std::optional<TimePoint<>> result() final;
    virtual void reset() final;

    virtual ~MassCombiner() {}

protected:
    /**
     * Calculate mass the answer.
     *
     * @param answer;
     * @param vector of locators by witch the answer was gotten.
     *
     * @return mass of the answer.
     */
    virtual double getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) = 0;

private:
    std::vector<std::pair<TimePoint<>, double>> data_;
};

};

#endif //PLOC_MASSCOMBINER_H
