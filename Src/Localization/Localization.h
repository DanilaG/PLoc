#ifndef PLOC_LOCALIZATION_H
#define PLOC_LOCALIZATION_H

#include <optional>
#include <utility>
#include <memory>
#include <vector>

#include "Combiner/Combiner.h"
#include "Combiner/FilteredMeanCombiner.h"
#include "Combiner/MeanCombiner.h"
#include "Combiner/MedianCombiner.h"
#include "Combiner/TimeSumCombiner.h"
#include "Combiner/TriangleCombiner.h"
#include "Geometric/Points/TimePoint.h"

namespace pl {
    /**
     * Direct methods of localization.
     *
     * Calculate signal location and time by direct methods.
     *
     * @param locator's position and time detection lightning as TimePoints or as vector of TimePoint.
     * @param signal propagation speed (c);
     * @param combiner of results.
     *
     * @return position and time of the signal or nil if localization has failed.
     */
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

    /* Approximate solution methods */

    /**
     * Calculate signal location and time of appearance by Elder-Mead method of minimization.
     *
     * @param vector of locators position and detection's time;
     * @param signal propagation speed (c);
     * @param start point for approximate;
     * @param maximal number of iteration;
     * @param accuracy for break and number of iteration while accuracy has unmutated before exit;
     * @param step for generation neighborhood points for start approximating;
     * @param alpha, gamma, rho, sigma: parameters of Elder-Mead method.
     *
     * @return position and time of the signal or nil if localization has failed.
     */
    std::optional<TimePoint<>> localizationByElderMeadMethod(const std::vector<TimePoint<>>& data,
                                                             double c,
                                                             const TimePoint<>& start,
                                                             unsigned int maxNumberIteration = 1000,
                                                             std::pair<double, unsigned int> breakParameters = {1e-16, 50},
                                                             double step = 0.1,
                                                             double alpha = 1,
                                                             double gamma = 2,
                                                             double rho = -0.5,
                                                             double sigma = 0.5);
}

#endif //PLOC_LOCALIZATION_H
