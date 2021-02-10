#ifndef PLOC_ERRORGENERATORS_H
#define PLOC_ERRORGENERATORS_H

#include <random>

/** Error generator protocol */
class ErrorGenerator {
public:
    ErrorGenerator() {
        std::random_device d;
        gen = std::mt19937(d());
    }

    /** Get error */
    virtual double get() = 0;
    double operator()() { return get(); }

protected:
    std::mt19937 gen;
};

/** Error generator with linear distribution */
class LinearErrorGenerator: public ErrorGenerator {
public:
    /** Constructor accepting minimum and maximum value of a error */
    LinearErrorGenerator(double min, double max): distribution_(min, max) {}

    double get() final {
        return distribution_(gen);
    }

protected:
    std::uniform_real_distribution<> distribution_;
};

/** Error generator with normal distribution */
class NormalErrorGenerator: public ErrorGenerator {
public:
    /** Constructor accepting mean and standard deviation of normal distribution */
    NormalErrorGenerator(double mean, double standardDeviation): distribution_(mean, standardDeviation) {}

    double get() final {
        return distribution_(gen);
    }

protected:
    std::normal_distribution<double> distribution_;
};

#endif //PLOC_ERRORGENERATORS_H
