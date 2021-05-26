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

Point<double> projectByEquirectangular(const SpherePoint<T>& location, double radius, double standardParallel) {
    return {radius * location.longitude * cos(standardParallel), radius * location.latitude};
}

Point<double> projectByKavrayskiy7(const SpherePoint<T>& location, double radius) {
    doyble k = location.latitude / M_PI;
    return {
        radius * 3.0 / 2.0 * location.longitude * sqrt(1.0 / 3.0 - (k * k)),
        radius * location.latitude
    };
}

Point<double> projectBySinusoidal(const SpherePoint<T>& location, double radius) {
    return {radius * location.longitude * cos(location.latitude), radius * location.latitude};
}

}

#endif //PLOC_SPHEREPOINT_H
