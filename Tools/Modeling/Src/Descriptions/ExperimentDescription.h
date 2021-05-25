#ifndef PLOC_EXPERIMENTDESCRIPTION_H
#define PLOC_EXPERIMENTDESCRIPTION_H

#include <memory>
#include <vector>

#include <Geometric/Points/Point.h>

/** Describe experiment scene */
struct Scene {
    /** Bound of a area under consideration */
    struct Bound {
        /** Min value of x and y coordinates the area */
        pl::Point<> min;

        /** Max value of x and y coordinates the area */
        pl::Point<> max;
    } bound;

    /** Signal propagation speed */
    double c;

    /** Positions of all detectors */
    std::vector<pl::Point<>> detectors;

    /** Signals position */
    std::vector<pl::Point<>> signals;
};

/** Description of random error generator */
class ErrorGeneratorDescription {
public:
    enum class GeneratorType: unsigned int {
        Linear,
        Normal
    };

    ErrorGeneratorDescription(GeneratorType generationType, bool isRelativeErrors): generatorType(generationType),
                                                                                    isRelativeErrors(isRelativeErrors) {}

    /** Type of generator */
    const GeneratorType generatorType;

    /** True, if generate errors relative values */
    const bool isRelativeErrors;
};

/** Linear generator */
class LinearErrorGeneratorDescription: public ErrorGeneratorDescription {
public:
    LinearErrorGeneratorDescription(bool isRelativeError): ErrorGeneratorDescription(GeneratorType::Linear,
                                                                                     isRelativeError) {}

    /** Min return value */
    double minValue;
    /** Max return value */
    double maxValue;
};

/** Normal generator */
class NormalErrorGeneratorDescription: public ErrorGeneratorDescription {
public:
    NormalErrorGeneratorDescription(bool isRelativeError): ErrorGeneratorDescription(GeneratorType::Normal,
                                                                                     isRelativeError) {}

    /** Mean of a normal distribution */
    double mean;
    /** Standard deviation of a normal distribution */
    double standardDeviation;
};

/** Describe of a experiment */
struct ExperimentDescription {
    /** Type of a experiment */
    enum class Type {
        /** Explore only grid in scene bounds */
        Grid,
        /** Explore only localization specified signals */
        Signal,
        /** Explore grid and signals */
        GridAndSignal
    };

    /** Type of localization algorithm */
    enum class LocalizationAlgoType {
        Direct,
        Quadrangle,
        QP
    };

    /** Type of combiner algorithm */
    enum class CombinerType {
        Mean,
        FilteredMean,
        Median,
        Triangle
    };

    /** Scene area sampling grid */
    struct GridSize {
        /** Number of nodes on the x axis in the grid */
        unsigned int width;

        /** Number of nodes on the y axis in the grid */
        unsigned int height;
    };

    Type type = Type::GridAndSignal;

    std::vector<Scene> scenes;

    /** Size scene grid */
    GridSize gridSize;

    /** Grid for signal localization type of an experiment */
    GridSize signalGridSize;

    /** Number of attempts experiments in each signal / node */
    unsigned int numberAttempts;

    std::shared_ptr<ErrorGeneratorDescription> cErrorGenerator;
    std::shared_ptr<ErrorGeneratorDescription> timeErrorGenerator;

    LocalizationAlgoType algoType;
    CombinerType combinerType;
};

#endif //PLOC_EXPERIMENTDESCRIPTION_H
