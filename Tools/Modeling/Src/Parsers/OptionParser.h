#ifndef PLOC_OPTIONPARSER_H
#define PLOC_OPTIONPARSER_H

#include <utility>
#include <optional>

#include <cxxopts.hpp>

#include "ExperimentDescription.h"
#include "ReportDescription.h"

/** Program arguments parser */
class OptionParser {
public:
    /** Parser result */
    struct Result {
    public:
        /** Data type */
        using Data = std::pair<ExperimentDescription, ReportDescription>;

        Result(const std::optional<Data>& data = std::nullopt): data_(data) {}

        /** Return true, if help requested */
        bool isHelpRequested() { return !data_.has_value(); }

        /** Return data, or make throw */
        Data data() { return data_.value(); }
    private:
        std::optional<Data> data_;
    };

    /** Parser initialization */
    static void init();

    /** Parses input arguments. In error case make throw */
    static Result pars(int argc, char** argv);

    /** Return help message for arguments */
    static std::string helpMessage() { return options_.help(); }

private:
    OptionParser() {}
    ~OptionParser() {}

    OptionParser(const OptionParser&) = delete;
    OptionParser& operator=(const OptionParser&) = delete;

    static bool isWasInited;
    static cxxopts::Options options_;
};


#endif //PLOC_OPTIONPARSER_H
