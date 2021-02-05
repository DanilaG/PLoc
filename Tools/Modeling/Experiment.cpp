#include <fstream>
#include <optional>

#include "Experiment.h"

#include <nlohmann/json.hpp>
#include <magic_enum.hpp>

pl::Point<> getPoint(nlohmann::json& json) {
    pl::Point<> point;
    point.x = json.at("x").get<double>();
    point.y = json.at("y").get<double>();
    return point;
}

Field getField(nlohmann::json& json) {
    Field field;
    field.max = getPoint(json.at("max"));
    field.min = getPoint(json.at("min"));
    field.detectors.clear();
    for (auto& i : json.at("detectors").items()) {
        field.detectors.push_back(getPoint(i.value()));
    }
    return field;
}

std::vector<Field> getFields(nlohmann::json& json) {
    std::vector<Field> fields;
    for (auto& i : json.items()) {
        nlohmann::json fieldJson;
        if (i.value().is_string()) {
            auto fileName = i.value().get<std::string>();
            std::ifstream file(fileName);
            if (!file.is_open()) {
                throw std::invalid_argument("File " + fileName + " with field wasn't open.");
            }
            file >> fieldJson;
            file.close();
        } else {
            fieldJson = i.value();
        }
        fields.push_back(getField(fieldJson));
    }
    return fields;
};

std::shared_ptr<ErrorGeneratorDescription> getLinearRandomGeneratorFromJson(nlohmann::json& json) {
    auto data = std::make_shared<LinearErrorGeneratorDescription>();
    data->maxValue = json.at("max_value").get<double>();
    data->minValue = json.at("min_value").get<double>();
    return data;
}

std::shared_ptr<ErrorGeneratorDescription> getNormalRandomGeneratorFromJson(nlohmann::json& json) {
    auto data = std::make_shared<NormalErrorGeneratorDescription>();
    data->mean = json.at("mean").get<double>();
    data->standardDeviation = json.at("standard_deviation").get<double>();
    return data;
}

std::shared_ptr<ErrorGeneratorDescription> getRandomGeneratorFromJson(nlohmann::json& json) {
    static const std::array<std::shared_ptr<ErrorGeneratorDescription> (*)(nlohmann::json&),
            magic_enum::enum_count<ErrorGeneratorDescription::GeneratorType>()> fabrica = {
            getLinearRandomGeneratorFromJson,
            getNormalRandomGeneratorFromJson
    };

    const std::string typeName = json.at("type").get<std::string>();
    auto type = magic_enum::enum_cast<ErrorGeneratorDescription::GeneratorType>(typeName);
    if (!type.has_value()) {
        throw std::invalid_argument("Wrong type of error generator.");
    }

    return fabrica[static_cast<unsigned int>(type.value())](json);
}

template <class T>
T getEnumFromJson(nlohmann::json& json, const std::string& paramName) {
    const std::string jsonValue = json.at(paramName).get<std::string>();
    auto optionalValue = magic_enum::enum_cast<T>(jsonValue);
    if (!optionalValue.has_value()) {
        throw std::invalid_argument("Field " + paramName + " have wrong argument: " +
                                    jsonValue + ".");
    }
    return optionalValue.value();
}

ExperimentDescription loadExperimentDescriptionFromJson(const std::string filePath) {
    /** Parameters names */
    static const std::string fieldsParamName = "fields";
    static const std::string gridParamName = "grid";
    static const std::string gridWidthParamName = "width";
    static const std::string gridHeightParamName = "height";
    static const std::string numberAttemptsInNodeParamName = "number_attempts";
    static const std::string cErrorGeneratorParamName = "c_error_generator";
    static const std::string timeErrorGeneratorParamName = "time_error_generator";
    static const std::string algoTypeParamName = "algorithm";
    static const std::string combinerTypeParamName = "combiner";
    /****/

    ExperimentDescription experiment;

    std::ifstream fileWithDescription(filePath);
    if (!fileWithDescription.is_open()) {
        throw std::invalid_argument("Json file not found!");
    }

    nlohmann::json json;
    try {
        fileWithDescription >> json;

        /** Fields */
        experiment.fields = getFields(json.at(fieldsParamName));

        /** Grid */
        auto jsonGrid = json.at(gridParamName);
        experiment.gridSize.width = jsonGrid.at(gridWidthParamName).get<unsigned int>();
        experiment.gridSize.height = jsonGrid.at(gridHeightParamName).get<unsigned int>();

        /** Attempts */
        experiment.numberAttemptsInNode = json.at(numberAttemptsInNodeParamName).get<unsigned int>();

        /** Error generators */
        experiment.cErrorGenerator = getRandomGeneratorFromJson(json.at(cErrorGeneratorParamName));
        experiment.timeErrorGenerator = getRandomGeneratorFromJson(json.at(timeErrorGeneratorParamName));

        /** Combiner type */
        experiment.algoType = getEnumFromJson<ExperimentDescription::LocalizationAlgoType>(json,
                                                                                           algoTypeParamName);

        /** Combiner type */
        experiment.combinerType = getEnumFromJson<ExperimentDescription::CombinerType>(json, combinerTypeParamName);
    } catch (std::exception& exc) {
        throw std::invalid_argument("Exception on JSON parsing:\n" + std::string(exc.what()));
    }

    return experiment;
}

