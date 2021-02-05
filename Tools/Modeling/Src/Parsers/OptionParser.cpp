#include "OptionParser.h"

#include <iostream>
#include <fstream>

#include "ExperimentJsonParser.h"

namespace opt_names {
    const std::string experiment = "experiment";
    const std::string out = "out";
    const std::string time = "time";
    const std::string distance = "distance";
    const std::string textReport = "text_report";
    const std::string help = "help";
};

void OptionParser::init() {
    if (!isWasInited) {
        options_.add_options()
                (
                        "e," + opt_names::experiment,
                        "Path to file with experiment description.",
                        cxxopts::value<std::string>()
                )
                (
                        "o," + opt_names::out,
                        "Output path.",
                        cxxopts::value<std::string>()->default_value("")
                )
                (
                        "t," + opt_names::time,
                        "Flag for using time measurement."
                )
                (
                        "d," + opt_names::distance,
                        "Flag for using distance measurement."
                )
                (
                        "r," + opt_names::textReport,
                        "Make text report."
                )
                (
                        "h," + opt_names::help,
                        "Get help message."
                );
        options_.parse_positional({opt_names::experiment, opt_names::out});
        isWasInited = true;
    }
}

OptionParser::Result OptionParser::pars(int argc, char** argv) {
    auto result = options_.parse(argc, argv);

    if (result.count(opt_names::help)) {
        return Result(std::nullopt);
    }

    ReportDescription report = {.measurers = {},
                                .useTextDescription = result[opt_names::textReport].as<bool>(),
                                .outPath = result[opt_names::out].as<std::string>() };

    if (result[opt_names::time].as<bool>()) {
        report.measurers.emplace_back(ReportDescription::Measurer::Time);
    }
    if (result[opt_names::distance].as<bool>()) {
        report.measurers.emplace_back(ReportDescription::Measurer::Dist);
    }

    const std::string inFileName= result[opt_names::experiment].as<std::string>();
    std::ifstream inFile(inFileName);
    if (!inFile.is_open()) {
        throw std::invalid_argument("Json file not found!");
    }
    nlohmann::json json;
    inFile >> json;
    ExperimentDescription experiment = getExperimentDescription(json);

    return Result(std::pair<ExperimentDescription, ReportDescription>(experiment, report));
}

bool OptionParser::isWasInited = false;

cxxopts::Options OptionParser::options_ =
        cxxopts::Options("PLocModeling", "A brief description");
