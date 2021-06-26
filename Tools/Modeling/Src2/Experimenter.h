#ifndef PLOC_EXPERIMENTER_H
#define PLOC_EXPERIMENTER_H

#include <utility>
#include <optional>
#include <memory>
#include <string>
#include <vector>

#include "Algorithm.h"
#include "Combiner.h"
#include "ErrorGenerator.h"
#include "Scene.h"

class Experimenter {
public:
    struct Grid {
        unsigned int width;
        unsigned int height;
    };

    void conduct();

    std::unique_ptr<Algorithm> algorithm;
    std::shared_ptr<ErrorGenerator> cErrorGenerator;
    std::shared_ptr<ErrorGenerator> timeErrorGenerator;
    std::unique_ptr<Scene> scene;

    std::optional<Grid> grid;
    std::optional<Scene::Position> signal;
    unsigned int numberAttempts;

    std::string outPutDir;

private:
    class DistanceVisitor: public Scene::Visitor {
    public:
        DistanceVisitor(const Scene::Position& from,
                        const Scene::Position& to): from_(from), to_(to) {}

        void onPlain(SceneOnPlain* scene) final;

        void onSphere(SceneOnSphere* scene) final;

        double countFor(Scene* scene);

        double result;
    private:
        Scene::Position from_;
        Scene::Position to_;
    };

    class FromSceneCoordinateVisitor: public Scene::Visitor {
    public:
        FromSceneCoordinateVisitor(Scene::Position position): position_(position) {}

        void onPlain(SceneOnPlain* scene) final;

        void onSphere(SceneOnSphere* scene) final;

        pl::Point<> translateFor(Scene* scene);

    private:
        Scene::Position position_;
        pl::Point<> result_;
    };

    class ToSceneCoordinateVisitor: public Scene::Visitor {
    public:
        ToSceneCoordinateVisitor(pl::TimePoint<> point): point_(point) {}

        void onPlain(SceneOnPlain* scene) final;

        void onSphere(SceneOnSphere* scene) final;

        Scene::PositionWithTime translateFor(Scene* scene);

    private:
        pl::TimePoint<> point_;
        Scene::PositionWithTime result_;
    };

    class CoordinateNamesVisitor: public Scene::Visitor {
    public:
        void onPlain(SceneOnPlain* scene) final { names = "x, y, time"; }

        void onSphere(SceneOnSphere* scene) final { names = "latitude, longitude, time"; }

        std::string names;
    };

    class ResultMat {
    public:
        ResultMat(Grid size): size_(size), data_(size.width * size.height, {0, 0}) {}

        void add(unsigned int x, unsigned int y, double value) {
            data_[getDataIndex(x, y)].first += value;
            data_[getDataIndex(x, y)].second++;
        }

        double get(unsigned int x, unsigned int y) const {
            return data_[getDataIndex(x, y)].first / data_[getDataIndex(x, y)].second;
        }

        bool hasValue(unsigned int x, unsigned int y) const { return data_[getDataIndex(x, y)].second != 0; }

        Grid size() const { return size_; }

    private:
        unsigned int getDataIndex(unsigned int x, unsigned int y) const { return y * size_.width + x; }

        Grid size_;
        std::vector<std::pair<double, unsigned int>> data_;
    };

    void conductForGrid();

    void conductForSignals();

    std::optional<Scene::PositionWithTime> makeAttempt(const Scene::Position& signal);

    void saveResultMat(const ResultMat& mat, const std::string& name);

    void saveSignalsResults(const std::vector<Scene::PositionWithTime>& results);

    void saveInSignals();

    void saveDetectors();

    void save(const std::vector<Scene::Position>& data, const std::string& fileName);

    std::string fullFileName(const std::string& fileName);
};


#endif //PLOC_EXPERIMENTER_H
