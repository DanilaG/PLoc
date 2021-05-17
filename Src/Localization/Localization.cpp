#include "Localization.h"

#define PL_ADD_LOCALIZATION_FUNC_WITH_DEFAULT_COMBINER(FUNC)                      \
std::optional<TimePoint<>> FUNC(const std::vector<TimePoint<>>& data, double c) { \
    MeanCombiner combiner;                                                        \
    return FUNC(data, c, combiner);                                               \
}

namespace pl {
    PL_ADD_LOCALIZATION_FUNC_WITH_DEFAULT_COMBINER(localizationByDirectMethod)
    PL_ADD_LOCALIZATION_FUNC_WITH_DEFAULT_COMBINER(localizationByQuadrangleMethod)
    PL_ADD_LOCALIZATION_FUNC_WITH_DEFAULT_COMBINER(localizationByQPMethod)
}