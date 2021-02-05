#include "OptionParser.h"

int main(int argc, char** argv) {
    OptionParser parser;
    auto result = parser(argc, argv);
    if (!result.has_value()) {
        if (parser.isHelpRequested()) {
            std::cout << parser.getHelpMessage() << std::endl;
            return 0;
        } else {
            std::string errorMessage;
            if (parser.error().has_value()) {
                errorMessage = parser.error().value().what;
            } else {
                errorMessage = "Unknown error in parser.";
            }
            std::cerr << errorMessage << std::endl;
        }
        return -1;
    }

    auto [experimentDescription, reportDescription] = result.value();

    return 0;
}
