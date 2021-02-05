#ifndef PLOC_OPTIONPARSER_H
#define PLOC_OPTIONPARSER_H

#include <utility>
#include <optional>

#include <cxxopts.hpp>

#include "Experiment.h"
#include "Report.h"

class OptionParser {
public:
    enum class ErrorsType {
        optionPars,
        file,
        experimentFilePars
    };

    struct Error {
        ErrorsType type;
        std::string what;
    };

    OptionParser();

    std::optional<std::pair<ExperimentDescription, ReportDescription>> getParams(int argc, char** argv);

    std::optional<std::pair<ExperimentDescription, ReportDescription>> operator()(int argc, char** argv) {
        return getParams(argc, argv);
    }

    bool isHelpRequested() { return isNeedHelpMessage_; }

    std::string getHelpMessage() { return options_.help(); }

    std::optional<Error> error() { return error_; };

private:
    cxxopts::Options options_;
    std::optional<Error> error_;
    bool isNeedHelpMessage_ = false;
};


#endif //PLOC_OPTIONPARSER_H
