#include "Executor/Executor.h"
#include "Parsers/OptionParser.h"

#include "Localization/Combiner/MeanCombiner.h"

int main(int argc, char** argv) {
    OptionParser::Result parserResult;
    try {
        OptionParser::init();
        parserResult = OptionParser::pars(argc, argv);
    } catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return -1;
    }

    if (parserResult.isHelpRequested()) {
        std::cout << OptionParser::helpMessage << std::endl;
        return 0;
    }

    auto [experimentDescription, reportDescription] = parserResult.data();
    auto experimentResult = conductExperiment(experimentDescription);

    return 0;
}
