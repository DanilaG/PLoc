#include "OptionParser.h"

#include <fstream>
#include <unordered_map>

#include <magic_enum.hpp>

#include "ExperimentJsonParser.h"

namespace opt_names {
    const std::string experiment = "experiment";
    const std::string out = "out";
    const std::string experimentType = "type";
    const std::string time = "time";
    const std::string distance = "distance";
    const std::string textReport = "text_report";
    const std::string help = "help";
};

std::string allPossibleValuesExperimentType() {
    auto names = magic_enum::enum_names<ExperimentDescription::Type>();
    std::string answer;
    for (auto& name : names) {
        answer += ("\"" + std::string(name) + "\" ");
    }
    answer.pop_back();
    return answer;
}

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
                        opt_names::experimentType,
                        "Type of an experiment. Possible values: " + allPossibleValuesExperimentType() + ".",
                        cxxopts::value<std::string>()->default_value(
                                    std::string(magic_enum::enum_name(ExperimentDescription::Type::GridAndSignal))
                                )
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

    auto experimentType =
            magic_enum::enum_cast<ExperimentDescription::Type>(result[opt_names::experimentType].as<std::string>());
    if (!experimentType.has_value()) {
        throw std::invalid_argument("Wrong experiment type. Possible types: " + allPossibleValuesExperimentType() + ".");
    }
    experiment.type = experimentType.value();

    return Result(std::pair<ExperimentDescription, ReportDescription>(experiment, report));
}

bool OptionParser::isWasInited = false;

cxxopts::Options OptionParser::options_ =
        cxxopts::Options("PLocModeling", "A brief description");
