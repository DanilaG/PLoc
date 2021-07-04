#ifndef PLOC_SCENE_H
#define PLOC_SCENE_H

#include <memory>
#include <utility>
#include <vector>

#include "Geometric/Points/SpherePoint.h"

class SceneOnPlain;
class SceneOnSphere;

class Scene {
public:
    typedef std::pair<double, double> Position;

    struct PositionWithTime: public Position {
        double time;
    };

    struct Restrictions {
        Position min;
        Position max;
    };

    struct Visitor {
    public:
        virtual void onPlain(SceneOnPlain* scene) = 0;
        virtual void onSphere(SceneOnSphere* scene) = 0;
    };

    virtual void accept(Visitor* visitor) = 0;
    virtual ~Scene() {}

    Restrictions restrictions;
    double c;
    std::vector<Position> detectors;
};

class SceneOnPlain: public Scene {
public:
    void accept(Visitor* visitor) final { visitor->onPlain(this); }
};

class SceneOnSphere: public Scene {
public:
    class ProjectByEquirectangular;
    class ProjectBySinusoidal;

    class Projection {
    public:
        struct Visitor {
        public:
            virtual void onEquirectangular(ProjectByEquirectangular* project) = 0;
            virtual void onSinusoidal(ProjectBySinusoidal* project) = 0;
        };

        virtual void accept(Visitor* visitor) = 0;

        virtual pl::Point<> to(const pl::SpherePoint& location, double radius) = 0;

        virtual pl::SpherePoint from(const pl::Point<double>& location, double radius) = 0;

        virtual ~Projection() {}
    };

    class ProjectByEquirectangular: public Projection {
    public:
        void accept(Visitor* visitor) final { visitor->onEquirectangular(this); }

        pl::Point<> to(const pl::SpherePoint& location, double radius) final {
            return pl::projectByEquirectangular(location, radius, 0);
        }

        pl::SpherePoint from(const pl::Point<double>& location, double radius) final {
            return pl::fromEquirectangular(location, radius, 0);
        }
    };

    class ProjectBySinusoidal: public Projection {
    public:
        void accept(Visitor* visitor) final { visitor->onSinusoidal(this); }

        pl::Point<> to(const pl::SpherePoint& location, double radius) final {
            return pl::projectBySinusoidal(location, radius);
        }

        pl::SpherePoint from(const pl::Point<double>& location, double radius) final {
            return pl::fromSinusoidal(location, radius);
        }
    };

    void accept(Visitor* visitor) final { visitor->onSphere(this); }

    std::unique_ptr<Projection> projection;
    double radius;
};

#endif //PLOC_SCENE_H
