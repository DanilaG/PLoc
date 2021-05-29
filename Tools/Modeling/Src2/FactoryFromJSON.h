#ifndef PLOC_FACTORYFROMJSON_H
#define PLOC_FACTORYFROMJSON_H

#include <string>

#include "Experimenter.h"

Experimenter createExperimenterFromJSON(const std::string& fileName);

#endif //PLOC_FACTORYFROMJSON_H
