#ifndef PLOC_REPORT_H
#define PLOC_REPORT_H

#include <memory>
#include <list>

/** Protocol for measuring experiment results */
class Measurer {
public:
    virtual ~Measurer() {}
};

class DistMeasurer: public Measurer {
public:
    DistMeasurer() {}
};

class TimeMeasurer: public Measurer {
public:
    TimeMeasurer() {}
};

/** Descript of a report */
struct ReportDescription {
    /** All measurers needed in report */
    std::list<std::unique_ptr<Measurer>> measurers;

    /** True, if need create a text description of a experiment */
    bool useTextDescription;

    /** Path for output files */
    std::string outPath = "";
};

#endif //PLOC_REPORT_H
