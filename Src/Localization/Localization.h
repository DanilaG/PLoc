#ifndef PLOC_LOCALIZATION_H
#define PLOC_LOCALIZATION_H

#include <optional>
#include <memory>
#include <vector>

#include "Combiner/Combiner.h"
#include "Combiner/MeanCombiner.h"
#include "Geometric/Points/TimePoint.h"

namespace pl {
    std::optional<TimePoint<>>
        localizationByDirectMethod(const std::vector<TimePoint<>>& data,
                                   double c,
                                   std::unique_ptr<Combiner> combiner = std::make_unique<MeanCombiner>());

    std::optional<TimePoint<>>
        localizationByQuadrangleMethod(const std::vector<TimePoint<>>& data,
                                       double c,
                                       std::unique_ptr<Combiner> combiner = std::make_unique<MeanCombiner>());

    std::optional<TimePoint<>>
        localizationByVectorMethod(const std::vector<TimePoint<>>& data,
                                   double c,
                                   std::unique_ptr<Combiner> combiner = std::make_unique<MeanCombiner>());
}

#endif //PLOC_LOCALIZATION_H
