#ifndef PLOC_EXPERIMENT_H
#define PLOC_EXPERIMENT_H

#include <memory>
#include <string>
#include <vector>

#include <Geometric/Points/Point.h>

/** Describe experiment field */
struct Field {
    /** Min value of x and y coordinates the field */
    Point<> min;

    /** Max value of x and y coordinates the field */
    Point<> max;

    /** Positions of all detectors */
    std::vector<Point<>> detectors;
};

/** Protocol for random error generator */
class RandomGenerator {
public:
    /** Name of generator */
    const std::string name;

    /** Get description the generator */
    virtual std::string toString() = 0;

    /** Get random error */
    virtual double get_error() = 0;

    /** Get random error */
    double operator()() {};

    virtual ~RandomGenerator() {};
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

    std::unique_ptr<RandomGenerator> cErrorGenerator;
    std::unique_ptr<RandomGenerator> timeErrorGenerator;

    LocalizationAlgoType algoType;
    CombinerType combinerType;
};

#endif //PLOC_EXPERIMENT_H
