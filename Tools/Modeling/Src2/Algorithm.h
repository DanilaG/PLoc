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
    virtual std::optional<pl::TimePoint<>> calculateFor(const std::vector<pl::TimePoint<>>&, double c) = 0;

    virtual ~Algorithm() {}
};

class ExactAlgorithm: public Algorithm {
public:
    ExactAlgorithm(std::shared_ptr<pl::Combiner> combiner): combiner(combiner) {}

    std::shared_ptr<pl::Combiner> combiner;
};

class DirectAlgorithm: public ExactAlgorithm {
public:
    DirectAlgorithm(std::shared_ptr<pl::Combiner> combiner): ExactAlgorithm(combiner) {}

    std::optional<pl::TimePoint<>> calculateFor(const std::vector<pl::TimePoint<>>& detectors,  double c) final {
        return pl::localizationByDirectMethod(detectors, c, *combiner);
    }
};

class QPAlgorithm: public ExactAlgorithm {
public:
    QPAlgorithm(std::shared_ptr<pl::Combiner> combiner): ExactAlgorithm(combiner) {}

    std::optional<pl::TimePoint<>> calculateFor(const std::vector<pl::TimePoint<>>& detectors,  double c) final {
        return pl::localizationByQPMethod(detectors, c, *combiner);
    }
};

class QuadAlgorithm: public ExactAlgorithm {
public:
    QuadAlgorithm(std::shared_ptr<pl::Combiner> combiner): ExactAlgorithm(combiner) {}

    std::optional<pl::TimePoint<>> calculateFor(const std::vector<pl::TimePoint<>>& detectors,  double c) final {
        return pl::localizationByQuadrangleMethod(detectors, c, *combiner);
    }
};

class ElderMeadAlgorithm: public Algorithm {
public:
    ElderMeadAlgorithm() {}

    std::optional<pl::TimePoint<>> calculateFor(const std::vector<pl::TimePoint<>>& detectors,  double c) final {
        pl::TimePoint<> start {0, 0, 0};
        if (startPointAlgorithm != nullptr) {
            auto result = startPointAlgorithm->calculateFor(detectors, c);
            if (result.has_value()) {
                start = result.value();
            }
        }
        return pl::localizationByElderMeadMethod(detectors,
                                                 c,
                                                 start,
                                                 numberIteration,
                                                 step,
                                                 alpha,
                                                 gamma,
                                                 rho,
                                                 sigma);
    }

    std::unique_ptr<Algorithm> startPointAlgorithm = nullptr;
    unsigned int numberIteration = 1000;
    double step = 0.1;
    double alpha = 1;
    double gamma = 2;
    double rho = -0.5;
    double sigma = 0.5;
};

#endif //PLOC_ALGORITHM_H
