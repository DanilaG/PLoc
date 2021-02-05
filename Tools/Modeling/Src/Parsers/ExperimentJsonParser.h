#ifndef PLOC_EXPERIMENTJSONPARSER_H
#define PLOC_EXPERIMENTJSONPARSER_H

#include <nlohmann/json.hpp>

#include "ExperimentDescription.h"

Field getField(nlohmann::json& json);
ExperimentDescription getExperimentDescription(nlohmann::json& json);

#endif //PLOC_EXPERIMENTJSONPARSER_H
