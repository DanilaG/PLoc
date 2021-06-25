#ifndef PLOC_LOCALIZATION_H
#define PLOC_LOCALIZATION_H

#include <optional>
#include <memory>
#include <vector>

#include "Combiner/Combiner.h"
#include "Combiner/MeanCombiner.h"
#include "Combiner/FilteredMeanCombiner.h"
#include "Combiner/MedianCombiner.h"
#include "Combiner/TriangleCombiner.h"
#include "Combiner/TimeSumCombiner.h"
#include "Geometric/Points/TimePoint.h"

namespace pl {
    std::optional<TimePoint<>> localizationByDirectMethod(const TimePoint<>& p0,
                                                          TimePoint<> p1,
                                                          TimePoint<> p2,
                                                          double c);
    std::optional<TimePoint<>> localizationByDirectMethod(const std::vector<TimePoint<>>& data, double c);
    std::optional<TimePoint<>> localizationByDirectMethod(const std::vector<TimePoint<>>& data,
                                                          double c,
                                                          Combiner& combiner);

    std::optional<TimePoint<>> localizationByQuadrangleMethod(const TimePoint<>& p0,
                                                              TimePoint<> p1,
                                                              TimePoint<> p2,
                                                              TimePoint<> p3,
                                                              double c);
    std::optional<TimePoint<>> localizationByQuadrangleMethod(const std::vector<TimePoint<>>& data, double c);
    std::optional<TimePoint<>> localizationByQuadrangleMethod(const std::vector<TimePoint<>>& data,
                                                              double c,
                                                              Combiner& combiner);

    std::optional<TimePoint<>> localizationByQPMethod(const TimePoint<>& p0,
                                                      TimePoint<> p1,
                                                      TimePoint<> p2,
                                                      double c);
    std::optional<TimePoint<>> localizationByQPMethod(const std::vector<TimePoint<>>& data, double c);
    std::optional<TimePoint<>> localizationByQPMethod(const std::vector<TimePoint<>>& data,
                                                      double c,
                                                      Combiner& combiner);

    std::optional<TimePoint<>> localizationByElderMeadMethod(const std::vector<TimePoint<>>& data,
                                                             double c,
                                                             const TimePoint<>& start,
                                                             unsigned int numberIteration = 1000,
                                                             double step = 0.1,
                                                             double alpha = 1,
                                                             double gamma = 2,
                                                             double rho = -0.5,
                                                             double sigma = 0.5);
}

#endif //PLOC_LOCALIZATION_H
