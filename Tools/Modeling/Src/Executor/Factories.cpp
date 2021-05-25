#include "Factories.h"

#include <unordered_map>

#include "Localization/Localization.h"

std::shared_ptr<ErrorGenerator> linearErrorGeneratorFactory(std::shared_ptr<ErrorGeneratorDescription> description) {
    auto linearDescription = std::static_pointer_cast<LinearErrorGeneratorDescription>(description);
    return std::make_shared<LinearErrorGenerator>(linearDescription->minValue,
                                                  linearDescription->maxValue,
                                                  linearDescription->isRelativeErrors);
}

std::shared_ptr<ErrorGenerator> normalErrorGeneratorFactory(std::shared_ptr<ErrorGeneratorDescription> description) {
    auto normalDescription = std::static_pointer_cast<NormalErrorGeneratorDescription>(description);
    return std::make_shared<NormalErrorGenerator>(normalDescription->mean,
                                                  normalDescription->standardDeviation,
                                                  description->isRelativeErrors);
}

std::shared_ptr<ErrorGenerator> errorGeneratorFactory(std::shared_ptr<ErrorGeneratorDescription> description) {
    std::unordered_map<ErrorGeneratorDescription::GeneratorType,
            std::shared_ptr<ErrorGenerator> (*)(std::shared_ptr<ErrorGeneratorDescription> description)> factories = {
                {ErrorGeneratorDescription::GeneratorType::Linear, linearErrorGeneratorFactory},
                {ErrorGeneratorDescription::GeneratorType::Normal, normalErrorGeneratorFactory}
            };

    return factories[description->generatorType](description);
}

std::shared_ptr<pl::Combiner> combinerFactory(const ExperimentDescription::CombinerType& combiner) {
    std::unordered_map<ExperimentDescription::CombinerType, std::shared_ptr<pl::Combiner>(*)()> factories = {
            {
                ExperimentDescription::CombinerType::Mean,
                [](){ return std::static_pointer_cast<pl::Combiner>(std::make_shared<pl::MeanCombiner>()); }
            },
            {
                    ExperimentDescription::CombinerType::FilteredMean,
                    [](){ return std::static_pointer_cast<pl::Combiner>(std::make_shared<pl::FilteredMeanCombiner>()); }
            },
            {
                    ExperimentDescription::CombinerType::Median,
                    [](){ return std::static_pointer_cast<pl::Combiner>(std::make_shared<pl::MedianCombiner>()); }
            },
            {
                    ExperimentDescription::CombinerType::Triangle,
                    [](){ return std::static_pointer_cast<pl::Combiner>(std::make_shared<pl::TriangleCombiner>()); }
            }
    };

    return factories[combiner]();
}

LocalizationFunc localizationAlgorithmFactory(const ExperimentDescription::LocalizationAlgoType& algoType) {
    std::unordered_map<ExperimentDescription::LocalizationAlgoType, LocalizationFunc> factories = {
            {ExperimentDescription::LocalizationAlgoType::Direct,     pl::localizationByDirectMethod    },
            {ExperimentDescription::LocalizationAlgoType::Quadrangle, pl::localizationByQuadrangleMethod},
            {ExperimentDescription::LocalizationAlgoType::QP,         pl::localizationByQPMethod        }
    };

    return factories[algoType];
}
