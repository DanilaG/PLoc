#ifndef PLOC_ALGORITHM_H
#define PLOC_ALGORITHM_H

#include <optional>
#include <vector>

#include "Localization/Localization.h"
#include "Geometric/Points/TimePoint.h"

class DirectAlgorithm;
class QPAlgorithm;
class QuadAlgorithm;

class Algorithm {
public:
    virtual std::optional<pl::TimePoint<>> calculate(const std::vector<pl::TimePoint<>>&, double, pl::Combiner&) = 0;

    virtual ~Algorithm() {}
};

class DirectAlgorithm: public Algorithm {
public:
    std::optional<pl::TimePoint<>> calculate(const std::vector<pl::TimePoint<>>& detectors,
                                             double c,
                                             pl::Combiner& combiner) final {
        return pl::localizationByDirectMethod(detectors, c, combiner);
    }
};

class QPAlgorithm: public Algorithm {
public:
    std::optional<pl::TimePoint<>> calculate(const std::vector<pl::TimePoint<>>& detectors,
                                             double c,
                                             pl::Combiner& combiner) final {
        return pl::localizationByQPMethod(detectors, c, combiner);
    }
};

class QuadAlgorithm: public Algorithm {
public:
    std::optional<pl::TimePoint<>> calculate(const std::vector<pl::TimePoint<>>& detectors,
                                             double c,
                                             pl::Combiner& combiner) final {
        return pl::localizationByQuadrangleMethod(detectors, c, combiner);
    }
};

#endif //PLOC_ALGORITHM_H
