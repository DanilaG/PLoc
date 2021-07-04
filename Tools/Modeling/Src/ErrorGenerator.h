#ifndef PLOC_ERRORGENERATOR_H
#define PLOC_ERRORGENERATOR_H

#include <random>

class LinearErrorGenerator;
class NormalErrorGenerator;

class ErrorGenerator {
public:
    ErrorGenerator();

    virtual double get() = 0;

    virtual ~ErrorGenerator() {}

protected:
    std::mt19937 gen_;
};

class LinearErrorGenerator: public ErrorGenerator {
public:
    LinearErrorGenerator(double min, double max): ErrorGenerator(), distribution_(min, max) {}

    double get() final;

private:
    std::uniform_real_distribution<> distribution_;
};

class NormalErrorGenerator: public ErrorGenerator {
public:
    NormalErrorGenerator(double mean, double standardDeviation): ErrorGenerator(),
                                                                 distribution_(mean, standardDeviation) {}

    double get() final;

private:
    std::normal_distribution<double> distribution_;
};

#endif //PLOC_ERRORGENERATOR_H
