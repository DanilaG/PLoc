#ifndef PLOC_TIMEPOINT_H
#define PLOC_TIMEPOINT_H

#include <string>

#include "Point.h"

namespace pl {
template <class P = double, class T = double>
class TimePoint : public Point<P> {
   public:
    TimePoint(const Point<P>& p, T t) : Point<P>(p), time(t) {}
    TimePoint(P x = 0, P y = 0, T t = 0)
        : Point<P>{x, y}, time(t) {}

    T time;
};

template <class P, class T>
TimePoint<P, T> operator+(const TimePoint<P, T>& a, const TimePoint<P, T>& b) {
    return TimePoint<P, T>(a.x + b.x, a.y + b.y, a.time + b.time);
}

template <class P, class T>
TimePoint<P, T> operator-(const TimePoint<P, T>& a, const TimePoint<P, T>& b) {
    return TimePoint<P, T>(a.x - b.x, a.y - b.y, a.time - b.time);
}

template <class P, class T>
std::string toString(const TimePoint<P, T>& p) {
    return std::string("x: ") + std::to_string(p.x) +
           " y: " + std::to_string(p.y) + " time: " + std::to_string(p.time);
}

}

#endif  // PLOC_TIMEPOINT_H
