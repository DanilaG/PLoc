#ifndef PLOC_EXPERIMENTDESCRIPTION_H
#define PLOC_EXPERIMENTDESCRIPTION_H

#include <memory>
#include <vector>

#include <Geometric/Points/Point.h>

/** Describe experiment field */
struct Field {
    /** Min value of x and y coordinates the field */
    pl::Point<> min;

    /** Max value of x and y coordinates the field */
    pl::Point<> max;

    /** Positions of all detectors */
    std::vector<pl::Point<>> detectors;
};

/** Description of random error generator */
class ErrorGeneratorDescription {
public:
    enum class GeneratorType: unsigned int {
        Linear,
        Normal
    };

    ErrorGeneratorDescription(GeneratorType generationType): generationType(generationType) {}

    /** Type of generator */
    const GeneratorType generationType;
};

/** Linear generator */
class LinearErrorGeneratorDescription: public ErrorGeneratorDescription {
public:
    LinearErrorGeneratorDescription(): ErrorGeneratorDescription(GeneratorType::Linear) {}

    /** Min return value */
    double minValue;
    /** Max return value */
    double maxValue;
};

/** Normal generator */
class NormalErrorGeneratorDescription: public ErrorGeneratorDescription {
public:
    NormalErrorGeneratorDescription(): ErrorGeneratorDescription(GeneratorType::Normal) {}

    /** Mean of a normal distribution */
    double mean;
    /** Standard deviation of a normal distribution */
    double standardDeviation;
};

/** Descript of a experiment */
struct ExperimentDescription {
    /** Type of localization algorithm */
    enum class LocalizationAlgoType {
        Direct,
        Quadrangle,
        Vector
    };

    /** Type of combiner algorithm */
    enum class CombinerType {
        Mean
    };

    /** Field sampling grid */
    struct GridSize {
        /** Number of nodes on the x axis in the grid */
        unsigned int width;

        /** Number of nodes on the y axis in the grid */
        unsigned int height;
    };

    std::vector<Field> fields;
    GridSize gridSize;

    /** Number of attempts experiments in each node */
    unsigned int numberAttemptsInNode;

    std::shared_ptr<ErrorGeneratorDescription> cErrorGenerator;
    std::shared_ptr<ErrorGeneratorDescription> timeErrorGenerator;

    LocalizationAlgoType algoType;
    CombinerType combinerType;
};

#endif //PLOC_EXPERIMENTDESCRIPTION_H
