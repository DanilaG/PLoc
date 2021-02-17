#include "Report.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <magic_enum.hpp>

/** Added all mass in a grid */
unsigned int gridMass(const GridResult& grid) {
    unsigned int massCounter = 0;
    for (unsigned int x = 0; x < grid.size().width; x++) {
        for (unsigned int y = 0; y < grid.size().height; y++) {
            massCounter += grid.massAnswer(x, y);
        }
    }
    return massCounter;
}

/** Compute mean value in grid */
double meanValue(const GridResult& grid) {
    double mean = 0;
    unsigned int massCounter = 0;

    for (unsigned int x = 0; x < grid.size().width; x++) {
        for (unsigned int y = 0; y < grid.size().height; y++) {
            if (grid.hasValue(x, y)) {
                mean += grid(x, y);
                massCounter += grid.massAnswer(x, y);
            }
        }
    }

    return mean / massCounter;
}

/** Compute min and max in a grid */
std::pair<double, double> minMax(const GridResult& grid) {
    if (grid.size().width == 0 && grid.size().height == 0) {
        return {0, 0};
    }

    std::pair<double, double> minMax = {grid(0, 0), grid(0, 0)};
    for (unsigned int x = 0; x < grid.size().width; x++) {
        for (unsigned int y = 0; y < grid.size().height; y++) {
            auto value = grid(x, y);
            minMax.first = std::min(minMax.first, value);
            minMax.second = std::max(minMax.second, value);
        }
    }

    return minMax;
}


/** Save detectors location in DAT format according:
  * http://grapherhelp.goldensoftware.com/WTOPICS/TOP_ASCIIFILES.htm */
void saveDetectorsInDatFormat(const std::vector<pl::Point<>>& detectors, const std::string& outFileName) {
    std::ofstream file(outFileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open " + outFileName + ".");
    }

    file << "\"x\", \"y\"" << std::endl;
    for (const auto& detector: detectors) {
        file << detector.x << ", " << detector.y << std::endl;
    }

    file.close();
}


/** Save grid data in GRD text format according:
  * http://grapherhelp.goldensoftware.com/subsys/ascii_grid_file_format.htm */
void saveGridInTextGrdFormat(const GridResult& grid,
                             const pl::Point<>& min,
                             const pl::Point<>& max,
                             const std::string& outFileName) {
    const std::string nanValue = "1.71041e38";
    const auto zMinMax = minMax(grid);

    std::ofstream file(outFileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open " + outFileName + ".");
    }

    file << "DSAA" << std::endl;
    file << grid.size().width << " " << grid.size().height << std::endl;
    file << min.x << " " << max.x << std::endl;
    file << min.y << " " << max.y << std::endl;
    file << zMinMax.first << " " << zMinMax.second << std::endl;

    for (unsigned int y = 0; y < grid.size().height; y++) {
        for (unsigned int x = 0; x < grid.size().width; x++) {
            file << (grid.hasValue(x, y) ? std::to_string(grid(x, y)) : nanValue ) << " ";
        }
        file << std::endl;
    }

    file.close();
}

/** Combine arguments for save functions */
struct ParametersForSave {
    const ExperimentDescription& experimentDescription;
    const std::vector<ExperimentResult>& experimentResults;
    unsigned int fieldIndex;
    const std::string& outPath;
};

/** Create .grd file with distance errors grid */
void saveDistanceErrorsGrid(const ParametersForSave& args) {
    saveGridInTextGrdFormat(args.experimentResults[args.fieldIndex].distance,
                            args.experimentDescription.fields[args.fieldIndex].min,
                            args.experimentDescription.fields[args.fieldIndex].max,
                            args.outPath + "Distance.grd");
}

/** Create .grd file with time errors grid */
void saveTimeErrorsGrid(const ParametersForSave& args) {
    saveGridInTextGrdFormat(args.experimentResults[args.fieldIndex].time,
                            args.experimentDescription.fields[args.fieldIndex].min,
                            args.experimentDescription.fields[args.fieldIndex].max,
                            args.outPath + "Time.grd");
}

/** Return space offset according line level */
std::string lineOffset(unsigned int level) {
    return std::string(4 * level, ' ');
}

/** Return name of errors type */
inline std::string getErrorsTypeName(bool isRelativeErrors) {
    return isRelativeErrors ? "relative" : "direct";
}

/** Make text description of a linear error generator */
std::string makeLinearErrorGeneratorDescription(const std::shared_ptr<ErrorGeneratorDescription>& description) {
    auto linearDescription = std::static_pointer_cast<LinearErrorGeneratorDescription>(description);
    return lineOffset(1) + "type: " + std::string(magic_enum::enum_name(linearDescription->generatorType)) + ";\n" +
           lineOffset(1) + "error type: " + getErrorsTypeName(linearDescription->isRelativeErrors) + ";\n" +
           lineOffset(1) + "min value: " + std::to_string(linearDescription->minValue) + ";\n" +
           lineOffset(1) + "max value: " + std::to_string(linearDescription->maxValue) + ".\n";
}

/** Make text description of a normal error generator */
std::string makeNormalErrorGeneratorDescription(const std::shared_ptr<ErrorGeneratorDescription>& description) {
    auto linearDescription = std::static_pointer_cast<NormalErrorGeneratorDescription>(description);
    return lineOffset(1) + "type: " + std::string(magic_enum::enum_name(linearDescription->generatorType)) + ";\n" +
           lineOffset(1) + "error type: " + getErrorsTypeName(linearDescription->isRelativeErrors) + ";\n" +
           lineOffset(1) + "mean: " + std::to_string(linearDescription->mean) + ";\n" +
           lineOffset(1) + "standard deviation: " + std::to_string(linearDescription->standardDeviation) + ".\n";
}

/** Make text description of a error generator */
std::string makeErrorGeneratorDescription(const std::shared_ptr<ErrorGeneratorDescription>& description) {
    std::unordered_map<ErrorGeneratorDescription::GeneratorType,
                       std::string (*)(const std::shared_ptr<ErrorGeneratorDescription>& description)>
    errorGeneratorDescriptions = {
            {ErrorGeneratorDescription::GeneratorType::Linear, makeLinearErrorGeneratorDescription},
            {ErrorGeneratorDescription::GeneratorType::Normal, makeNormalErrorGeneratorDescription}
    };

    return errorGeneratorDescriptions[description->generatorType](description);
}

/** Create a text report of an experiment */
void saveTextReport(const ParametersForSave& args) {
    const std::string outFileName = args.outPath + "TextReport.txt";

    std::ofstream file(outFileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open " + outFileName + ".");
    }

    auto field = args.experimentDescription.fields[args.fieldIndex];
    unsigned int numberGeneratedSignals = args.experimentDescription.gridSize.width *
                                          args.experimentDescription.gridSize.height *
                                          args.experimentDescription.numberAttemptsInNode;

    file << "Signal propagation speed: " << field.c << ".\n" << std::endl;

    file << "Signal propagation speed error generator:\n"
         << makeErrorGeneratorDescription(args.experimentDescription.cErrorGenerator) << std::endl;

    file << "Time error generator:\n"
         << makeErrorGeneratorDescription(args.experimentDescription.timeErrorGenerator) << std::endl;

    file << "Localization algorithm: " << magic_enum::enum_name(args.experimentDescription.algoType) << ".\n";
    file << "Combiner algorithm: "
         << magic_enum::enum_name(args.experimentDescription.combinerType) << ".\n" << std::endl;

    file << "Number attempts of location in each node: "
         << args.experimentDescription.numberAttemptsInNode << ".\n" << std::endl;

    file << "Mean distance error: " << meanValue(args.experimentResults[args.fieldIndex].distance) << ".\n";
    file << "Mean time error: " << meanValue(args.experimentResults[args.fieldIndex].time) << ".\n" << std::endl;

    file << "Percent of localized signals: "
         << gridMass(args.experimentResults[args.fieldIndex].time) / double(numberGeneratedSignals) * 100 << "%.\n"
         << std::endl;

    file << "Full localization time: " << args.experimentResults[args.fieldIndex].duration << " milliseconds.\n";
    file << "Mean localization time of a signal: "
         << args.experimentResults[args.fieldIndex].duration / numberGeneratedSignals << " milliseconds.\n"
         << std::endl;

    file.close();
}

void makeReport(const ExperimentDescription& experimentDescription,
                const std::vector<ExperimentResult>& experimentResults,
                const ReportDescription& reportDescription) {

    std::unordered_map<ReportDescription::Measurer, void (*)(const ParametersForSave&)> measurerSaverFactories = {
            {ReportDescription::Measurer::Dist, saveDistanceErrorsGrid },
            {ReportDescription::Measurer::Time, saveTimeErrorsGrid }
    };

    for (unsigned int fieldIndex = 0; fieldIndex < experimentResults.size(); fieldIndex++) {
        std::string outPath = reportDescription.outPath + std::string(1, std::filesystem::path::preferred_separator);

        /** Create directories with reports */
        if (experimentResults.size() > 1) {
            outPath += ("Field" + std::to_string(fieldIndex) +
                        std::string(1, std::filesystem::path::preferred_separator));
            std::filesystem::path dir(outPath);
            if (!std::filesystem::exists(dir)) {
                std::filesystem::create_directory(dir);
            }
        }

        ParametersForSave dataSaving = {experimentDescription, experimentResults, fieldIndex, outPath};

        for (const auto &i: reportDescription.measurers) {
            measurerSaverFactories[i](dataSaving);
        }

        if (!reportDescription.measurers.empty()) {
            saveDetectorsInDatFormat(experimentDescription.fields[fieldIndex].detectors, outPath + "Detectors.dat");
        }

        if (reportDescription.useTextDescription) {
            saveTextReport(dataSaving);
        }
    }
}