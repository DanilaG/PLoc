#ifndef PLOC_SPHEREPOINT_H
#define PLOC_SPHEREPOINT_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "Point.h"

namespace pl {
class SpherePoint {
public:
    double latitude;
    double longitude;
};

Point<double> projectByEquirectangular(const SpherePoint& location, double radius, double standardParallel);
SpherePoint fromEquirectangular(const Point<double>& location, double radius, double standardParallel);

Point<double> projectBySinusoidal(const SpherePoint& location, double radius);
SpherePoint fromSinusoidal(const Point<double>& location, double radius);

double dist(const SpherePoint& a, const SpherePoint& b, double radius);

}

#endif //PLOC_SPHEREPOINT_H
