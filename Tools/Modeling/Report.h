#ifndef PLOC_REPORT_H
#define PLOC_REPORT_H

#include <memory>
#include <list>

/** Protocol for measuring experiment results */
class Measurer {
public:
    virtual ~Measurer() {}
};

/** Descript of a report */
struct ReportDescription {
    /** All measurers needed in report */
    std::list<std::unique_ptr<Measurer>> measurers;

    /** True, if need create a text description of a experiment */
    bool useTextDescription;
};

#endif //PLOC_REPORT_H
