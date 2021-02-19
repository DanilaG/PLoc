#include "Executor.h"

#include <chrono>

#include "Factories.h"

/** Time detection a signal by a detector */
inline double timeDetection(const pl::Point<>& signalLocation, const pl::Point<>& detectorLocation, double c) {
    return pl::dist(signalLocation, detectorLocation) / c;
}

/** Modeling detection a signal by detectors */
std::vector<pl::TimePoint<>> detection(const std::vector<pl::Point<>>& detectors,
                                       const pl::Point<>& signalLocation,
                                       double c,
                                       std::shared_ptr<ErrorGenerator> cErrorGenerator,
                                       std::shared_ptr<ErrorGenerator> timeErrorGenerator) {
    std::vector<pl::TimePoint<>> result(detectors.size());
    for (unsigned int i = 0; i < result.size(); i++) {
        result[i] = { detectors[i],
                      timeErrorGenerator->applyErrorTo(
                              timeDetection(signalLocation, detectors[i], cErrorGenerator->applyErrorTo(c))
                      )};
    }
    return result;
}

std::vector<ExperimentResult> conductExperiment(const ExperimentDescription& experiment) {
    /** Error generators */
    auto cErrorGenerator = errorGeneratorFactory(experiment.cErrorGenerator);
    auto timeErrorGenerator = errorGeneratorFactory(experiment.timeErrorGenerator);

    /** Localization algorithms */
    auto combiner = combinerFactory(experiment.combinerType);
    auto localizationFunc = localizationAlgorithmFactory(experiment.algoType);

    GridResult::Size gridSize = {.width = experiment.gridSize.width, .height = experiment.gridSize.height};
    std::vector<ExperimentResult> answer;

    for (auto& scene: experiment.scenes) {
        /** Experiment result for current scene */
        ExperimentResult result(gridSize);

        /** Grid step for the scene area */
        pl::Point<> gridStep = {
            .x = (scene.bound.max.x - scene.bound.min.x) / (gridSize.width),
            .y = (scene.bound.max.y - scene.bound.min.y) / (gridSize.height)
        };

        for (unsigned int xStep = 0; xStep < gridSize.width; xStep++) {
            for (unsigned int yStep = 0; yStep < gridSize.height; yStep++) {
                /** Exact signal location */
                pl::Point<> signalLocation = {
                        .x = (xStep + 0.5) * gridStep.x + scene.bound.min.x,
                        .y = (yStep + 0.5) * gridStep.y + scene.bound.min.y
                };

                for (unsigned int attempt = 0; attempt < experiment.numberAttempts; attempt++) {
                    auto detections = detection(scene.detectors,
                                                signalLocation,
                                                scene.c,
                                                cErrorGenerator,
                                                timeErrorGenerator);

                    /** Localization */
                    auto startTime = std::chrono::high_resolution_clock::now();
                    auto localizationResult = localizationFunc(detections,
                                                               scene.c,
                                                               *combiner);
                    auto endTime = std::chrono::high_resolution_clock::now();

                    /** Experiment add duration */
                    auto ml = std::chrono::duration_cast< std::chrono::nanoseconds>(endTime - startTime) / 1.0e6;
                    result.duration +=
                            ml.count();
                    /** Add new result */
                    if (localizationResult.has_value()) {
                        result.distance.add(xStep, yStep, pl::dist(localizationResult.value(), signalLocation));
                        result.time.add(xStep, yStep, abs(localizationResult.value().time));
                    }
                }
            }
        }

        answer.push_back(result);
    }

    return answer;
}
