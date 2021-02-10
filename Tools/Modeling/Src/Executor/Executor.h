#ifndef PLOC_EXECUTOR_H
#define PLOC_EXECUTOR_H

#include "ExperimentDescription.h"
#include "ExperimentResult.h"

#include <vector>

/** Conduct experiment according to a description */
std::vector<ExperimentResult> conductExperiment(const ExperimentDescription& experiment);

#endif //PLOC_EXECUTOR_H
