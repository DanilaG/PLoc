#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include "Src2/FactoryFromJSON.h"

namespace opt_names {
    const std::string input = "input";
    const std::string output = "output";
    const std::string help = "help";
}

int main(int argc, char** argv) {
    cxxopts::Options options = cxxopts::Options("PLocModeling", "Tool for localization modelling.");
    options.add_options()
            (
                    "i," + opt_names::input,
                    "Path to JSON file with a experiment description.",
                    cxxopts::value<std::string>()
            )
            (
                    "o," + opt_names::output,
                    "Path for output directory.",
                    cxxopts::value<std::string>()->default_value(".")
            )
            (
                    "h," + opt_names::help,
                    "Show the help message."
            );
    options.parse_positional({opt_names::input, opt_names::output});
    auto parserResult = options.parse(argc, argv);

    if (parserResult.count(opt_names::help)) {
        std::cout << options.help() << std::endl;
    }

    Experimenter experimenter = createExperimenterFromJSON(parserResult[opt_names::input].as<std::string>());
    experimenter.outPutDir = parserResult[opt_names::output].as<std::string>();
    experimenter.conduct();

    return 0;
}
