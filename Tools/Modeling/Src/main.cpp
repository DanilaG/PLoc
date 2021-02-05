#include "Parsers/OptionParser.h"

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

    return 0;
}
