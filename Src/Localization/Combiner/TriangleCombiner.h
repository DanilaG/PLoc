#ifndef PLOC_TRIANGLECOMBINER_H
#define PLOC_TRIANGLECOMBINER_H

#include <utility>
#include <vector>

#include "Combiner.h"

namespace pl {
class TriangleCombiner: public Combiner {
public:
    void add(const TimePoint<>& answer, const std::vector<Point<>>& locators) final;

    std::optional<TimePoint<>> result() final;

    void reset() final;

    ~TriangleCombiner() {}

private:
    std::vector<std::pair<TimePoint<>, double>> data_;
};

}

#endif //PLOC_TRIANGLECOMBINER_H
