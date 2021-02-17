#ifndef PLOC_ERRORGENERATORS_H
#define PLOC_ERRORGENERATORS_H

#include <random>

/** Error generator protocol */
class ErrorGenerator {
public:
    /** Constructor accepting type of an errors (relative to a value if true) */
    ErrorGenerator(bool isRelativeErrors) {
        std::random_device d;
        gen_ = std::mt19937(d());
        isRelativeErrors_ = isRelativeErrors;
    }

    /** Return value with error */
    virtual double applyErrorTo(const double value) {
        double error = getError();
        return value + (isRelativeErrors_ ? value * error : error);
    }
    double operator()(const double value) { return applyErrorTo(value); }

protected:
    /** Standard generator */
    std::mt19937 gen_;

    /** Count an error relative to a value */
    bool isRelativeErrors_;

    /** Generate error value */
    virtual double getError() = 0;
};

/** Error generator with linear distribution */
class LinearErrorGenerator: public ErrorGenerator {
public:
    /** Constructor accepting minimum, maximum value of an errors and type of an errors */
    LinearErrorGenerator(double min, double max, bool isRelativeErrors): ErrorGenerator(isRelativeErrors),
                                                                         distribution_(min, max) {}

protected:
    double getError() final {
        return distribution_(gen_);
    }

protected:
    std::uniform_real_distribution<> distribution_;
};

/** Error generator with normal distribution */
class NormalErrorGenerator: public ErrorGenerator {
public:
    /** Constructor accepting mean, standard deviation of normal distribution and type of an errors */
    NormalErrorGenerator(double mean,
                         double standardDeviation,
                         bool isRelativeErrors): ErrorGenerator(isRelativeErrors),
                                                distribution_(mean, standardDeviation) {}

protected:
    double getError() final {
        return distribution_(gen_);
    }

protected:
    std::normal_distribution<double> distribution_;
};

#endif //PLOC_ERRORGENERATORS_H
