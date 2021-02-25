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
std::pair<double, double> minMax(const GridProtocol& grid) {
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
void saveGridInTextGrdFormat(const GridProtocol& grid,
                             const pl::Point<>& min,
                             const pl::Point<>& max,
                             const std::string& outFileName,
                             std::string (*getElement)(const GridProtocol&, unsigned int, unsigned int)) {
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
            file << getElement(grid, x, y) << " ";
        }
        file << std::endl;
    }

    file.close();
}

/** Return GridResult value or NAN in GRD text format */
std::string getGritResultOutValue(const GridProtocol& grid, unsigned int x, unsigned int y) {
    const std::string nanValue = "1.71041e38";
    return static_cast<const GridResult*>(&grid)->hasValue(x, y) ? std::to_string(grid(x, y)) : nanValue;
}

/** Save GridResult in GRD text format */
void saveGridResultInTextGrdFormat(const GridResult& grid,
                                   const pl::Point<>& min,
                                   const pl::Point<>& max,
                                   const std::string& outFileName) {
    saveGridInTextGrdFormat(grid, min, max, outFileName, getGritResultOutValue);
}

/** Return Grid value or NAN in GRD text format */
std::string getGritOutValue(const GridProtocol& grid, unsigned int x, unsigned int y) {
    return std::to_string(grid(x, y));
}

/** Save Grid in GRD text format */
void saveGridInTextGrdFormat(const Grid& grid,
                             const pl::Point<>& min,
                             const pl::Point<>& max,
                             const std::string& outFileName) {
    saveGridInTextGrdFormat(grid, min, max, outFileName, getGritOutValue);
}

/** Combine arguments for save functions */
struct ParametersForSave {
    const ExperimentDescription& experimentDescription;
    const std::vector<ExperimentResult>& experimentResults;
    unsigned int scenedIndex;
    const std::string& outPath;
};

/** Create .grd file with distance errors grid */
void saveDistanceErrorsGrid(const ParametersForSave& args) {
    if (args.experimentResults[args.scenedIndex].grid.has_value()) {
        saveGridResultInTextGrdFormat(args.experimentResults[args.scenedIndex].grid->distance,
                                      args.experimentDescription.scenes[args.scenedIndex].bound.min,
                                      args.experimentDescription.scenes[args.scenedIndex].bound.max,
                                      args.outPath + "Distance.grd");
    }
}

/** Create .grd file with time errors grid */
void saveTimeErrorsGrid(const ParametersForSave& args) {
    if (args.experimentResults[args.scenedIndex].grid.has_value()) {
        saveGridResultInTextGrdFormat(args.experimentResults[args.scenedIndex].grid->time,
                                      args.experimentDescription.scenes[args.scenedIndex].bound.min,
                                      args.experimentDescription.scenes[args.scenedIndex].bound.max,
                                      args.outPath + "Time.grd");
    }
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

    auto scene = args.experimentDescription.scenes[args.scenedIndex];
    unsigned int numberGeneratedSignals = args.experimentDescription.gridSize.width *
                                          args.experimentDescription.gridSize.height *
                                          args.experimentDescription.numberAttempts;

    file << "Signal propagation speed: " << scene.c << ".\n" << std::endl;

    file << "Signal propagation speed error generator:\n"
         << makeErrorGeneratorDescription(args.experimentDescription.cErrorGenerator) << std::endl;

    file << "Time error generator:\n"
         << makeErrorGeneratorDescription(args.experimentDescription.timeErrorGenerator) << std::endl;

    file << "Localization algorithm: " << magic_enum::enum_name(args.experimentDescription.algoType) << ".\n";
    file << "Combiner algorithm: "
         << magic_enum::enum_name(args.experimentDescription.combinerType) << ".\n" << std::endl;

    file << "Number attempts of location in each signal / node: "
         << args.experimentDescription.numberAttempts << ".\n" << std::endl;

    /** Grid text report part */
    if (args.experimentResults[args.scenedIndex].grid.has_value()) {
        file << "Mean distance error: " << meanValue(args.experimentResults[args.scenedIndex].grid->distance) << ".\n";
        file << "Mean time error: " << meanValue(args.experimentResults[args.scenedIndex].grid->time) << ".\n"
             << std::endl;

        file << "Percent of localized signals: "
             << gridMass(args.experimentResults[args.scenedIndex].grid->time) / double(numberGeneratedSignals) * 100
             << "%.\n" << std::endl;

        file << "Full localization time: " << args.experimentResults[args.scenedIndex].grid->duration
             << " milliseconds.\n";
        file << "Mean localization time of a signal: "
             << args.experimentResults[args.scenedIndex].grid->duration / numberGeneratedSignals << " milliseconds.\n"
             << std::endl;
    }

    file.close();
}

/** Create new directory if it necessary */
std::string createDir(std::string path, const std::string& dirName) {
    path += (dirName + std::string(1, std::filesystem::path::preferred_separator));
    std::filesystem::path dir(path);
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directory(dir);
    }
    return path;
}

/** Save localized signals */
void saveLocalizedSignals(const SignalExperimentResult& result, const std::string& outFileName) {
    std::ofstream file(outFileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open " + outFileName + ".");
    }

    file << "\"x\", \"y\", \"time\"" << std::endl;

    for(const auto& signal : result.detected) {
        file << signal.x << " " << signal.y << " " << signal.time << std::endl;
    }

    file.close();
}

/** Save grid scatter of localized signal */
void saveSignalGrid(const SignalExperimentResult& result,
                    const ExperimentDescription::GridSize& signalGridSize,
                    const std::string& outFileName) {
    if (!result.detected.empty()) {
        pl::Point<> min = result.detected.front();
        pl::Point<> max = result.detected.front();
        for (const auto& localization : result.detected) {
            min.x = std::min(min.x, localization.x);
            min.y = std::min(min.y, localization.y);
            max.x = std::max(max.x, localization.x);
            max.y = std::max(max.y, localization.y);
        }

        Grid grid(GridProtocol::Size{signalGridSize.width, signalGridSize.height});
        const pl::Point<> step = {(max.x - min.x) / (signalGridSize.width - 1),
                                  (max.y - min.y) / (signalGridSize.height - 1)};

        for (unsigned int x = 0; x < grid.size().width; x++) {
            for (unsigned int y = 0; y < grid.size().height; y++) {
                grid(x, y) = 0;
            }
        }

        for (const auto& localization : result.detected) {
            const pl::Point<int> unit = {
                    int(ceil((localization.x - min.x) / step.x)),
                    int(ceil((localization.y - min.y) / step.y))
            };
            grid(unit.x, unit.y) += 1;
        }

        saveGridInTextGrdFormat(grid, min, max, outFileName);
    }
}

/** Make localized signals report of a experiment */
void makeLocalizedSignalsReport(const ParametersForSave& args) {
    if (args.experimentDescription.type != ExperimentDescription::Type::Signal &&
        args.experimentDescription.type != ExperimentDescription::Type::GridAndSignal) {
        return;
    }

    auto& signalsResult = args.experimentResults[args.scenedIndex].signal;
    for (unsigned int signalIndex = 0; signalIndex < signalsResult.size(); signalIndex++) {
        std::string outPath = args.outPath + std::string(1, std::filesystem::path::preferred_separator);
        if (signalsResult.size() > 1) {
            outPath = createDir(outPath, "Signal" + std::to_string(signalIndex));
        }

        saveLocalizedSignals(signalsResult[signalIndex], outPath + "LocalizedSignals.dat");
        saveSignalGrid(signalsResult[signalIndex],
                       args.experimentDescription.signalGridSize,
                       outPath + "LocalizedSignalsGrid.dat");
    }
}

void makeReport(const ExperimentDescription& experimentDescription,
                const std::vector<ExperimentResult>& experimentResults,
                const ReportDescription& reportDescription) {

    std::unordered_map<ReportDescription::Measurer, void (*)(const ParametersForSave&)> measurerSaverFactories = {
            {ReportDescription::Measurer::Dist, saveDistanceErrorsGrid },
            {ReportDescription::Measurer::Time, saveTimeErrorsGrid }
    };

    for (unsigned int sceneIndex = 0; sceneIndex < experimentResults.size(); sceneIndex++) {
        std::string outPath = reportDescription.outPath + std::string(1, std::filesystem::path::preferred_separator);
        if (experimentResults.size() > 1) {
            outPath = createDir(outPath, "Scene" + std::to_string(sceneIndex));
        }

        ParametersForSave dataSaving = {experimentDescription, experimentResults, sceneIndex, outPath};

        for (const auto &i: reportDescription.measurers) {
            measurerSaverFactories[i](dataSaving);
        }

        if (!reportDescription.measurers.empty()) {
            saveDetectorsInDatFormat(experimentDescription.scenes[sceneIndex].detectors, outPath + "Detectors.dat");
        }

        if (reportDescription.useTextDescription) {
            saveTextReport(dataSaving);
        }

        makeLocalizedSignalsReport(dataSaving);
    }
}