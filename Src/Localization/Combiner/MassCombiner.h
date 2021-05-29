#ifndef PLOC_MASSCOMBINER_H
#define PLOC_MASSCOMBINER_H

#include "Combiner.h"

#include <utility>
#include <vector>

namespace pl {
class MassCombiner : public Combiner {
public:
    void add(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) final;
    std::optional<TimePoint<>> result() final;
    virtual void reset() final;

    virtual ~MassCombiner() {}

protected:
    virtual double getMass(const TimePoint<>& answer, const std::vector<TimePoint<>>& locators) = 0;

private:
    std::vector<std::pair<TimePoint<>, double>> data_;
};

};

#endif //PLOC_MASSCOMBINER_H
