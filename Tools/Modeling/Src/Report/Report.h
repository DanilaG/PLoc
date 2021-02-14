#ifndef PLOC_REPORT_H
#define PLOC_REPORT_H

#include "ExperimentDescription.h"
#include "ExperimentResult.h"
#include "ReportDescription.h"

/** Make report of a experiment */
void makeReport(const ExperimentDescription& experimentDescription,
                const std::vector<ExperimentResult>& experimentResults,
                const ReportDescription& reportDescription);

#endif //PLOC_REPORT_H
