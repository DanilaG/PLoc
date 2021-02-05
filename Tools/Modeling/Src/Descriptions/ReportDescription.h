#ifndef PLOC_REPORTDESCRIPTION_H
#define PLOC_REPORTDESCRIPTION_H

#include <memory>
#include <list>

/** Description of a report */
struct ReportDescription {
    enum class Measurer {
        Time,
        Dist
    };

    /** All measurers needed in report */
    std::list<Measurer> measurers;

    /** True, if need create a text description of a experiment */
    bool useTextDescription;

    /** Path for output files */
    std::string outPath = "";
};

#endif //PLOC_REPORTDESCRIPTION_H
