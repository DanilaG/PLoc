#include "ExperimentJsonParser.h"

#include <fstream>

#include <magic_enum.hpp>

/** Parameters names */
namespace json_param_name {
    namespace experiment {
        const std::string fields = "fields";
        const std::string grid = "grid";
        const std::string gridWidth = "width";
        const std::string gridHeight = "height";
        const std::string numberAttemptsInNode = "number_attempts";
        const std::string cErrorGenerator = "c_error_generator";
        const std::string timeErrorGenerator = "time_error_generator";
        const std::string algoType = "algorithm";
        const std::string combinerType = "combiner";
    }

    namespace norm_distrib {
        const std::string mean = "mean";
        const std::string standardDeviation = "standard_deviation";
    }

    namespace linear_distrib {
        const std::string minValue = "min_value";
        const std::string maxValue = "max_value";
    }

    namespace field {
        const std::string min = "min";
        const std::string max = "max";
        const std::string c = "c";
        const std::string detectors = "detectors";
    }

    namespace point {
        const std::string x = "x";
        const std::string y = "y";
    }
}

pl::Point<> getPoint(nlohmann::json& json) {
    pl::Point<> point;
    point.x = json.at(json_param_name::point::x).get<double>();
    point.y = json.at(json_param_name::point::y).get<double>();
    return point;
}

Field getField(nlohmann::json& json) {
    Field field;
    field.min = getPoint(json.at(json_param_name::field::min));
    field.max = getPoint(json.at(json_param_name::field::max));
    field.c = json.at(json_param_name::field::c).get<double>();
    field.detectors.clear();
    for (auto& i : json.at(json_param_name::field::detectors).items()) {
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

std::shared_ptr<ErrorGeneratorDescription> getLinearRandomGenerator(nlohmann::json& json) {
    auto data = std::make_shared<LinearErrorGeneratorDescription>();
    data->maxValue = json.at(json_param_name::linear_distrib::maxValue).get<double>();
    data->minValue = json.at(json_param_name::linear_distrib::minValue).get<double>();
    return data;
}

std::shared_ptr<ErrorGeneratorDescription> getNormalRandomGenerator(nlohmann::json& json) {
    auto data = std::make_shared<NormalErrorGeneratorDescription>();
    data->mean = json.at(json_param_name::norm_distrib::mean).get<double>();
    data->standardDeviation = json.at(json_param_name::norm_distrib::standardDeviation).get<double>();
    return data;
}

std::shared_ptr<ErrorGeneratorDescription> getRandomGenerator(nlohmann::json& json) {
    static const std::array<std::shared_ptr<ErrorGeneratorDescription> (*)(nlohmann::json&),
            magic_enum::enum_count<ErrorGeneratorDescription::GeneratorType>()> fabrica = {
            getLinearRandomGenerator,
            getNormalRandomGenerator
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

ExperimentDescription getExperimentDescription(nlohmann::json& json) {
    ExperimentDescription experiment;

    try {
        /** Fields */
        experiment.fields = getFields(json.at(json_param_name::experiment::fields));

        /** Grid */
        auto jsonGrid = json.at(json_param_name::experiment::grid);
        experiment.gridSize.width = jsonGrid.at(json_param_name::experiment::gridWidth).get<unsigned int>();
        experiment.gridSize.height = jsonGrid.at(json_param_name::experiment::gridHeight).get<unsigned int>();

        /** Attempts */
        experiment.numberAttemptsInNode =
                json.at(json_param_name::experiment::numberAttemptsInNode).get<unsigned int>();

        /** Error generators */
        experiment.cErrorGenerator = getRandomGenerator(json.at(json_param_name::experiment::cErrorGenerator));
        experiment.timeErrorGenerator = getRandomGenerator(json.at(json_param_name::experiment::timeErrorGenerator));

        /** Combiner type */
        experiment.algoType =
                getEnumFromJson<ExperimentDescription::LocalizationAlgoType>(json,
                                                                             json_param_name::experiment::algoType);

        /** Combiner type */
        experiment.combinerType =
                getEnumFromJson<ExperimentDescription::CombinerType>(json,
                                                                     json_param_name::experiment::combinerType);
    } catch (std::exception& exc) {
        throw std::invalid_argument("Exception on experiment description parsing:\n" + std::string(exc.what()));
    }

    return experiment;
}
