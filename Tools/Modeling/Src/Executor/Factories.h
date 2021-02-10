#ifndef PLOC_FACTORIES_H
#define PLOC_FACTORIES_H

#include <functional>
#include <memory>

#include "Localization/Combiner/Combiner.h"
#include "Localization/Combiner/MeanCombiner.h"
#include "ErrorGenerators.h"
#include "ExperimentDescription.h"

/** Type of function localization */
using LocalizationFunc = std::function<std::optional<pl::TimePoint<>>(const std::vector<pl::TimePoint<>>&,
                                                                      double,
                                                                      std::unique_ptr<pl::Combiner>)>;

/** Factories */
std::shared_ptr<ErrorGenerator> linearErrorGeneratorFactory(std::shared_ptr<ErrorGeneratorDescription> description);
std::shared_ptr<ErrorGenerator> normalErrorGeneratorFactory(std::shared_ptr<ErrorGeneratorDescription> description);
std::shared_ptr<ErrorGenerator> errorGeneratorFactory(std::shared_ptr<ErrorGeneratorDescription> description);
std::shared_ptr<pl::Combiner> combinerFactory(const ExperimentDescription::CombinerType& combiner);
LocalizationFunc localizationAlgorithmFactory(const ExperimentDescription::LocalizationAlgoType& algoType);

#endif //PLOC_FACTORIES_H
