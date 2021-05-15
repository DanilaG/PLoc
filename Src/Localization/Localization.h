#ifndef PLOC_LOCALIZATION_H
#define PLOC_LOCALIZATION_H

#include <optional>
#include <memory>
#include <vector>

#include "Combiner/Combiner.h"
#include "Combiner/MeanCombiner.h"
#include "Combiner/FilteredMeanCombiner.h"
#include "Combiner/MedianCombiner.h"
#include "Geometric/Points/TimePoint.h"

namespace pl {
    std::optional<TimePoint<>> localizationByDirectMethod(const std::vector<TimePoint<>>& data, double c);
    std::optional<TimePoint<>> localizationByDirectMethod(const std::vector<TimePoint<>>& data,
                                                          double c,
                                                          Combiner& combiner);

    std::optional<TimePoint<>> localizationByQuadrangleMethod(const std::vector<TimePoint<>>& data, double c);
    std::optional<TimePoint<>> localizationByQuadrangleMethod(const std::vector<TimePoint<>>& data,
                                                              double c,
                                                              Combiner& combiner);

    std::optional<TimePoint<>> localizationByVectorMethod(const std::vector<TimePoint<>>& data, double c);
    std::optional<TimePoint<>> localizationByVectorMethod(const std::vector<TimePoint<>>& data,
                                                          double c,
                                                          Combiner& combiner);
}

#endif //PLOC_LOCALIZATION_H
