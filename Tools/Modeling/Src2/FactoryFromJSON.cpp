#include "FactoryFromJSON.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "Experimenter.h"

/** Parameters names */
namespace json_param_name {
    namespace experiment {
        const std::string scene = "scene";
        const std::string grid = "grid";
        const std::string gridWidth = "width";
        const std::string gridHeight = "height";
        const std::string signal = "signal";
        const std::string numberAttemptsInNode = "number_attempts";
        const std::string cErrorGenerator = "c_error_generator";
        const std::string timeErrorGenerator = "time_error_generator";
        const std::string algorithm = "algorithm";
    }

    namespace alg {
        const std::string name = "name";
        const std::string combiner = "combiner";
        const std::string step = "step";
        const std::string alpha = "alpha";
        const std::string gamma = "gamma";
        const std::string rho = "rho";
        const std::string sigma = "sigma";
        const std::string numberIteration = "number_iteration";
        const std::string algorithm = "algorithm";
    }

    namespace error_gen {
        const std::string type = "type";
        const std::string isRelativeErrors = "relative_errors";

        namespace norm_distrib {
            const std::string mean = "mean";
            const std::string standardDeviation = "standard_deviation";
        }

        namespace linear_distrib {
            const std::string minValue = "min_value";
            const std::string maxValue = "max_value";
        }
    }

    namespace scene {
        namespace restrictions {
            const std::string min = "min";
            const std::string max = "max";
        }
        const std::string restrictionsName = "restrictions";
        const std::string c = "c";
        const std::string detectors = "detectors";
        const std::string radius = "radius";
        const std::string projection = "projection";
    }

    namespace point {
        namespace plant {
            const std::string x = "x";
            const std::string y = "y";
        }
        namespace sphere {
            const std::string latitude = "latitude";
            const std::string longitude = "longitude";
        }
    }
}

Scene::Position getPosition(nlohmann::json& json, const std::pair<std::string, std::string>& names) {
    return {
            json.at(names.first).get<double>(),
            json.at(names.second).get<double>()
    };
}

void fillScene(nlohmann::json& json, const std::pair<std::string, std::string>& coordinatesName, Scene* scene) {
    scene->c = json.at(json_param_name::scene::c).get<double>();
    for (auto& i : json.at(json_param_name::scene::detectors).items()) {
        scene->detectors.push_back(getPosition(i.value(), coordinatesName));
    }

    scene->restrictions.min = getPosition(
            json.at(json_param_name::scene::restrictionsName).at(json_param_name::scene::restrictions::min),
            coordinatesName
    );

    scene->restrictions.max = getPosition(
            json.at(json_param_name::scene::restrictionsName).at(json_param_name::scene::restrictions::max),
            coordinatesName
    );
}

Scene* createSceneOnPlainFromJSON(nlohmann::json& json) {
    SceneOnPlain* scene = new SceneOnPlain;
    fillScene(json,
              { json_param_name::point::plant::x, json_param_name::point::plant::y },
              scene);
    return scene;
}

Scene* createSceneOnSphereFromJSON(nlohmann::json& json) {
    const std::unordered_map<std::string, SceneOnSphere::Projection*(*)()> projectionsFactory = {
            {
                "Equirectangular",
                [](){ return static_cast<SceneOnSphere::Projection*>(new SceneOnSphere::ProjectByEquirectangular); }
            },
            {
                "Sinusoidal",
                [](){ return static_cast<SceneOnSphere::Projection*>(new SceneOnSphere::ProjectBySinusoidal); }
            }
    };

    SceneOnSphere* scene = new SceneOnSphere;
    fillScene(json,
              { json_param_name::point::sphere::latitude, json_param_name::point::sphere::longitude },
              scene);
    scene->radius = json.at(json_param_name::scene::radius).get<double>();
    auto it = projectionsFactory.find(json.at(json_param_name::scene::projection).get<std::string>());
    if (it == projectionsFactory.end()) {
        throw std::invalid_argument("Wrong projection!");
    }
    scene->projection = std::unique_ptr<SceneOnSphere::Projection>(it->second());
    return scene;
}

Scene* createSceneFromJSON(nlohmann::json& json) {
    if (json.contains(json_param_name::scene::radius) ||
        json.contains(json_param_name::scene::projection)) {
        return createSceneOnSphereFromJSON(json);
    }

    return createSceneOnPlainFromJSON(json);
}

ErrorGenerator* createLinearErrorGenerator(nlohmann::json& json) {
    const double min = json.at(json_param_name::error_gen::linear_distrib::minValue).get<double>();
    const double max = json.at(json_param_name::error_gen::linear_distrib::maxValue).get<double>();
    return new LinearErrorGenerator(min, max);
}

ErrorGenerator* createNormalErrorGenerator(nlohmann::json& json) {
    const double mean = json.at(json_param_name::error_gen::norm_distrib::mean).get<double>();
    const double standardDeviation = json.at(json_param_name::error_gen::norm_distrib::standardDeviation).get<double>();
    return new NormalErrorGenerator(mean, standardDeviation);
}

ErrorGenerator* createErrorGenerator(nlohmann::json& json) {
    if (json.contains(json_param_name::error_gen::norm_distrib::standardDeviation)) {
        return createNormalErrorGenerator(json);
    }

    return createLinearErrorGenerator(json);
}

pl::Combiner* createCombiner(nlohmann::json& json) {
    const std::unordered_map<std::string, pl::Combiner*(*)()> combinersFactory = {
            {
                    "Mean",
                    [](){ return static_cast<pl::Combiner*>(new pl::MeanCombiner); }
            },
            {
                    "FilteredMean",
                    [](){ return static_cast<pl::Combiner*>(new pl::FilteredMeanCombiner); }
            },
            {
                    "Median",
                    [](){ return static_cast<pl::Combiner*>(new pl::MedianCombiner); }
            },
            {
                    "Triangle",
                    [](){ return static_cast<pl::Combiner*>(new pl::TriangleCombiner); }
            },
            {
                    "TimeSum",
                    [](){ return static_cast<pl::Combiner*>(new pl::TimeSumCombiner); }
            }
    };

    auto it = combinersFactory.find(json.get<std::string>());
    if (it == combinersFactory.end()) {
        throw std::invalid_argument("Wrong combiner!");
    }
    return it->second();
}

Algorithm* createAlgorithm(nlohmann::json& json) {
    const std::unordered_map<std::string, Algorithm*(*)(nlohmann::json& json)> algorithmsFactory = {
            {
                    "Direct",
                    [](nlohmann::json& json) {
                        pl::Combiner* combiner = createCombiner(json.at(json_param_name::alg::combiner));
                        return static_cast<Algorithm*>(new DirectAlgorithm(std::shared_ptr<pl::Combiner>(combiner)));
                    }
            },
            {
                    "QP",
                    [](nlohmann::json& json) {
                        pl::Combiner* combiner = createCombiner(json.at(json_param_name::alg::combiner));
                        return static_cast<Algorithm*>(new QPAlgorithm(std::shared_ptr<pl::Combiner>(combiner)));
                    }
            },
            {
                    "Quadrangle",
                    [](nlohmann::json& json) {
                        pl::Combiner* combiner = createCombiner(json.at(json_param_name::alg::combiner));
                        return static_cast<Algorithm*>(new QuadAlgorithm(std::shared_ptr<pl::Combiner>(combiner)));
                    }
            },
            {
                    "ElderMead",
                    [](nlohmann::json& json) {
                        auto algorithm = new ElderMeadAlgorithm();
                        if (json.contains(json_param_name::alg::algorithm)) {
                            algorithm->startPointAlgorithm =
                                    std::unique_ptr<Algorithm>(createAlgorithm(json.at(json_param_name::alg::algorithm)));
                        }
                        if (json.contains(json_param_name::alg::step)) {
                            algorithm->step = json.at(json_param_name::alg::step).get<double>();
                        }
                        if (json.contains(json_param_name::alg::numberIteration)) {
                            algorithm->numberIteration = json.at(json_param_name::alg::numberIteration).get<unsigned>();
                        }
                        if (json.contains(json_param_name::alg::alpha)) {
                            algorithm->alpha = json.at(json_param_name::alg::alpha).get<double>();
                        }
                        if (json.contains(json_param_name::alg::gamma)) {
                            algorithm->gamma = json.at(json_param_name::alg::gamma).get<double>();
                        }
                        if (json.contains(json_param_name::alg::rho)) {
                            algorithm->rho = json.at(json_param_name::alg::rho).get<double>();
                        }
                        if (json.contains(json_param_name::alg::sigma)) {
                            algorithm->sigma = json.at(json_param_name::alg::sigma).get<double>();
                        }
                        return static_cast<Algorithm*>(algorithm);
                    }
            }
    };

    auto it = algorithmsFactory.find(json.at(json_param_name::alg::name).get<std::string>());
    if (it == algorithmsFactory.end()) {
        throw std::invalid_argument("Wrong algorithm!");
    }
    return it->second(json);
}

class CoordinateNamesVisitor: public Scene::Visitor {
public:
    void onPlain(SceneOnPlain* scene) final {
        coordinatesName_ = { json_param_name::point::plant::x, json_param_name::point::plant::y };
    }

    void onSphere(SceneOnSphere* scene) final {
        coordinatesName_ = { json_param_name::point::sphere::latitude, json_param_name::point::sphere::longitude };
    }

    std::pair<std::string, std::string> getName(Scene* scene) {
        scene->accept(this);
        return coordinatesName_;
    }

private:
    std::pair<std::string, std::string> coordinatesName_;
};

Experimenter createExperimenterFromJSON(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Json file not found!");
    }

    nlohmann::json json;
    file >> json;

    Experimenter experimenter;

    try {
        experimenter.scene = std::unique_ptr<Scene>(createSceneFromJSON(json.at(json_param_name::experiment::scene)));

        if (json.contains(json_param_name::experiment::grid)) {
            auto jsonGrid = json.at(json_param_name::experiment::grid);
            experimenter.grid = Experimenter::Grid{
                    jsonGrid.at(json_param_name::experiment::gridWidth).get<unsigned int>(),
                    jsonGrid.at(json_param_name::experiment::gridHeight).get<unsigned int>()
            };
        }

        if (json.contains(json_param_name::experiment::signal)) {
            experimenter.signal = getPosition(
                    json.at(json_param_name::experiment::signal),
                    CoordinateNamesVisitor().getName(experimenter.scene.get())
            );
        }

        experimenter.numberAttempts = json.at(json_param_name::experiment::numberAttemptsInNode).get<double>();

        experimenter.cErrorGenerator = std::unique_ptr<ErrorGenerator>(
                createErrorGenerator(json.at(json_param_name::experiment::cErrorGenerator))
        );

        experimenter.timeErrorGenerator = std::unique_ptr<ErrorGenerator>(
                createErrorGenerator(json.at(json_param_name::experiment::timeErrorGenerator))
        );

        experimenter.algorithm = std::unique_ptr<Algorithm>(
                createAlgorithm(json.at(json_param_name::experiment::algorithm))
        );

    } catch (std::exception& exc) {
        throw std::invalid_argument("Exception on experiment description parsing:\n" + std::string(exc.what()));
    }

    return experimenter;
}
