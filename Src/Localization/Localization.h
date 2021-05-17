#ifndef PLOC_LOCALIZATION_H
#define PLOC_LOCALIZATION_H

#include <optional>
#include <memory>
#include <vector>

#include "Combiner/Combiner.h"
#include "Combiner/MeanCombiner.h"
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
}

#endif //PLOC_LOCALIZATION_H
