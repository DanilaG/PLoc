#include "Executor/Executor.h"
#include "Parsers/OptionParser.h"
#include "Report/Report.h"

int main(int argc, char** argv) {
    OptionParser::Result parserResult;
    try {
        OptionParser::init();
        parserResult = OptionParser::pars(argc, argv);

        if (parserResult.isHelpRequested()) {
            std::cout << OptionParser::helpMessage << std::endl;
            return 0;
        }

        auto [experimentDescription, reportDescription] = parserResult.data();
        auto experimentResult = conductExperiment(experimentDescription);

        makeReport(experimentDescription, experimentResult, reportDescription);
    } catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return -1;
    }

    return 0;
}
