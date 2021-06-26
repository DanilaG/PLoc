#include "Experimenter.h"

#include <iostream>
#include <fstream>

void Experimenter::conduct() {
    conductForSignals();
    conductForGrid();

    saveDetectors();
}

void Experimenter::conductForGrid() {
    if (!grid.has_value()) {
        return;
    }

    ResultMat distResult(grid.value());
    ResultMat timeResult(grid.value());

    Scene::Position step = {
            (scene->restrictions.max.first - scene->restrictions.min.first) / grid.value().width,
            (scene->restrictions.max.second - scene->restrictions.min.second) / grid.value().height
    };

    for (unsigned int first = 0; first < grid.value().width; first++) {
        for (unsigned int second = 0; second < grid.value().height; second++) {
            std::cout << "Counted: " << (first * grid.value().height + second) / double(grid.value().width * grid.value().height) * 100.0 << "%" << std::endl;
            Scene::Position signal = {
                    (first + 0.5) * step.first + scene->restrictions.min.first,
                    (second + 0.5) * step.second + scene->restrictions.min.second
            };
            for (unsigned int attempt = 0; attempt < numberAttempts; attempt++) {
                auto location = makeAttempt(signal);

                if (location.has_value()) {
                    distResult.add(
                            first,
                            second,
                      DistanceVisitor(signal, location.value()).countFor(scene.get())
                    );
                    timeResult.add(first, second, abs(location->time));
                }
            }
        }
    }

    saveResultMat(distResult, "Distance.grd");
    saveResultMat(timeResult, "Time.grd");
}

void Experimenter::conductForSignals() {
    if (!signal.has_value()) {
        return;
    }

    Scene::Position signal = this->signal.value();
    std::vector<Scene::PositionWithTime> results;

    for (unsigned int attempt = 0; attempt < numberAttempts; attempt++) {
        auto location = makeAttempt(signal);

        if (location.has_value()) {
            results.push_back(location.value());
        }
    }

    saveSignalsResults(results);
    saveInSignals();
}

std::optional<Scene::PositionWithTime> Experimenter::makeAttempt(const Scene::Position& signal) {

    std::vector<pl::TimePoint<>> detectors;
    for (auto& detectorPosition: scene->detectors) {
        pl::Point<> detectorInMap = FromSceneCoordinateVisitor(detectorPosition).translateFor(scene.get());
        double cWithError = scene->c + cErrorGenerator->get();
        double time = DistanceVisitor(detectorPosition, signal).countFor(scene.get()) / cWithError;
        detectors.emplace_back(
                detectorInMap.x,
                detectorInMap.y,
                time + timeErrorGenerator->get()
        );
    }

    auto location = algorithm->calculateFor(detectors, scene->c);

    return location.has_value() ?
           std::optional<Scene::PositionWithTime>(
                   ToSceneCoordinateVisitor(location.value()).translateFor(scene.get())) :
           std::nullopt;
}

void Experimenter::saveResultMat(const ResultMat& mat, const std::string& name) {
    auto outFileName = fullFileName(name);
    std::ofstream file(outFileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open " + outFileName + ".");
    }

    double min = 0;
    double max = 0;
    for (unsigned int first = 0; first < mat.size().width; first++) {
        for (unsigned int second = 0; second < mat.size().height; second++) {
            if (mat.hasValue(first, second)) {
                min = mat.get(first, second);
                max = min;
            }
        }
    }
    for (unsigned int first = 0; first < mat.size().width; first++) {
        for (unsigned int second = 0; second < mat.size().height; second++) {
            if (mat.hasValue(first, second)) {
                min = std::min(min, mat.get(first, second));
                max = std::max(max, mat.get(first, second));
            }
        }
    }

    file << "DSAA" << std::endl;
    file << grid->width << " " << grid->height << std::endl;
    file << scene->restrictions.min.first << " " << scene->restrictions.max.first<< std::endl;
    file << scene->restrictions.min.second << " " << scene->restrictions.max.second<< std::endl;
    file << min << " " << max << std::endl;

    const std::string nanValue = "1.71041e38";
    for (unsigned int second = 0; second < mat.size().height; second++) {
        for (unsigned int first = 0; first < mat.size().width; first++) {
            file << (mat.hasValue(first, second) ? std::to_string(mat.get(first, second)) : nanValue) << " ";
        }
        file << std::endl;
    }

    file.close();
}

void Experimenter::saveSignalsResults(const std::vector<Scene::PositionWithTime>& results) {
    auto outFileName = fullFileName("LocalizedSignals.dat");
    std::ofstream file(outFileName);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open " + outFileName + ".");
    }

    CoordinateNamesVisitor coordinateNamesVisitor;
    scene->accept(&coordinateNamesVisitor);
    file << coordinateNamesVisitor.names << std::endl;

    for (const auto& position: results) {
        file << position.first << ", " << position.second << ", " << position.time << std::endl;
    }

    file.close();
}

void Experimenter::saveInSignals() {
    save({signal.value()}, "Signals.dat");
}

void Experimenter::saveDetectors() {
    save(scene->detectors, "Detectors.dat");
}

void Experimenter::save(const std::vector<Scene::Position>& data, const std::string& fileName) {
    auto outFileFullName = fullFileName(fileName);
    std::ofstream file(outFileFullName);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open " + outFileFullName + ".");
    }

    CoordinateNamesVisitor coordinateNamesVisitor;
    scene->accept(&coordinateNamesVisitor);
    file << coordinateNamesVisitor.names << std::endl;

    for (const auto& position: data) {
        file << position.first << ", " << position.second << std::endl;
    }

    file.close();
}

std::string Experimenter::fullFileName(const std::string& fileName) {
    return outPutDir + std::string(1, std::filesystem::path::preferred_separator) + fileName;
}

void Experimenter::DistanceVisitor::onPlain(SceneOnPlain* scene) {
    pl::Point<> from = { from_.first, from_.second };
    pl::Point<> to = { to_.first, to_.second };
    result = pl::dist(from, to);
}

void Experimenter::DistanceVisitor::onSphere(SceneOnSphere* scene)  {
    pl::SpherePoint from = { from_.first, from_.second };
    pl::SpherePoint to = { to_.first, to_.second };
    result = pl::dist(from, to, scene->radius);
}

double Experimenter::DistanceVisitor::countFor(Scene* scene) {
    scene->accept(this);
    return result;
}

void Experimenter::FromSceneCoordinateVisitor::onPlain(SceneOnPlain* scene) {
    result_ = { position_.first, position_.second };
}

void Experimenter::FromSceneCoordinateVisitor::onSphere(SceneOnSphere* scene) {
    result_ = scene->projection->to({position_.first, position_.second}, scene->radius);
}

pl::Point<> Experimenter::FromSceneCoordinateVisitor::translateFor(Scene* scene) {
    scene->accept(this);
    return result_;
}

void Experimenter::ToSceneCoordinateVisitor::onPlain(SceneOnPlain* scene) {
    result_ = { { point_.x, point_.y }, point_.time };
}

void Experimenter::ToSceneCoordinateVisitor::onSphere(SceneOnSphere* scene)  {
    auto spherePoint = scene->projection->from(point_, scene->radius);
    result_ = { { spherePoint.latitude, spherePoint.longitude }, point_.time };
}

Scene::PositionWithTime Experimenter::ToSceneCoordinateVisitor::translateFor(Scene* scene) {
    scene->accept(this);
    return result_;
}
