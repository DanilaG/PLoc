#include "SpherePoint.h"

#include <iostream>

namespace pl {
double toRadian(double degrees) {
    return degrees * M_PI / 180;
}

double toDegrees(double radians) {
    return radians * 180 / M_PI;
}

Point<double> projectByEquirectangular(const SpherePoint& location, double radius, double standardParallel) {
    return { radius * toRadian(location.longitude) * cos(toRadian(standardParallel)),
             radius * toRadian(location.latitude) };
}

SpherePoint fromEquirectangular(const Point<double>& location, double radius, double standardParallel) {
    return { toDegrees(location.y / radius),
             toDegrees(location.x / (radius * cos(toRadian(standardParallel)))) };
}

Point<double> projectBySinusoidal(const SpherePoint& location, double radius) {
    return { radius * toRadian(location.longitude) * cos(toRadian(location.latitude)),
             radius * toRadian(location.latitude) };
}

SpherePoint fromSinusoidal(const Point<double>& location, double radius) {
    double latitude = location.y / radius;
    return { toDegrees(latitude),
             toDegrees(location.x / (radius * cos(latitude))) };
}

double dist(const SpherePoint &a, const SpherePoint &b, double radius) {
    double longDelta = (a.longitude - b.longitude);
    double latDelta = (a.latitude - b.latitude);
    double k = cos(toRadian((a.latitude + a.latitude) / 2.0));
    return radius * toRadian(sqrt(longDelta * longDelta * k * k + latDelta * latDelta));
}

}
