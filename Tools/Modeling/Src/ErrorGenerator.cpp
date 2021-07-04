#include "ErrorGenerator.h"

ErrorGenerator::ErrorGenerator() {
    std::random_device d;
    gen_ = std::mt19937(d());
}

double LinearErrorGenerator::get() {
    return distribution_(gen_);
}

double NormalErrorGenerator::get() {
    return distribution_(gen_);
}
