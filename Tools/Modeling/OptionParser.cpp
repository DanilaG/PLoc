#include "OptionParser.h"

namespace opt_names {
    const std::string experiment = "experiment";
    const std::string out = "out";
    const std::string time = "time";
    const std::string distance = "distance";
    const std::string textReport = "text_report";
    const std::string help = "help";
};

OptionParser::OptionParser() : options_("PLocModeling", "A brief description") {
    options_.add_options()
             ("e," + opt_names::experiment, "Param bar", cxxopts::value<std::string>())
             ("o," + opt_names::out, "Param bar", cxxopts::value<std::string>()->default_value(""))
             ("t," + opt_names::time, "Param foo")
             ("d," + opt_names::distance, "Param foo")
             ("r," + opt_names::textReport, "Print usage")
            ("h," + opt_names::help, "Print usage");

    options_.parse_positional({opt_names::experiment, opt_names::out});
}

std::optional<std::pair<ExperimentDescription, ReportDescription>> OptionParser::getParams(int argc, char** argv) {
    try {
        auto result = options_.parse(argc, argv);
        isNeedHelpMessage_ = result.count(opt_names::help);

        ReportDescription report = {.measurers = {},
                                    .useTextDescription = result[opt_names::textReport].as<bool>(),
                                    .outPath = result[opt_names::out].as<std::string>() };
        if (result[opt_names::time].as<bool>()) {
            report.measurers.emplace_back(ReportDescription::Measurer::Time);
        }
        if (result[opt_names::distance].as<bool>()) {
            report.measurers.emplace_back(ReportDescription::Measurer::Dist);
        }


        ExperimentDescription experiment =
                loadExperimentDescriptionFromJson(result[opt_names::experiment].as<std::string>());

        return std::pair<ExperimentDescription, ReportDescription>(std::move(experiment), report);
    } catch (const std::exception& e) {
        error_ = Error({ErrorsType::optionPars, e.what()});
        return std::nullopt;
    }
}
