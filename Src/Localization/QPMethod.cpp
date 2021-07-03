#include "Localization.h"

#include "LocalizationTools.h"

namespace pl {
std::optional<TimePoint<>> localizationByQPMethod(const TimePoint<>& p0,
                                                  TimePoint<> p1,
                                                  TimePoint<> p2,
                                                  double c) {
    /* Local vars were named according to the formulas in the documentation. */
    p1 = p1 - p0;
    p2 = p2 - p0;
    const unsigned int numberPoints = 3;

    const double cSqr = sqr(c);
    const double v = (p1.x * p2.y - p2.x * p1.y);

    if (v == 0) {
        return std::nullopt;
    }

    const double r[numberPoints] = {0, dist(p1), dist(p2)};
    const double rSqr[numberPoints] = {0, sqr(r[1]), sqr(r[2])};
    const double tSqr[numberPoints] = {0, sqr(p1.time), sqr(p2.time)};
    const double q_[numberPoints] = {0, (rSqr[1] - cSqr * tSqr[1]) / 2, (rSqr[2] - cSqr * tSqr[2]) / 2};

    const double k = (p1.x * p2.x + p1.y * p2.y);
    const double A = (rSqr[2] * tSqr[1] - 2 * k * p1.time * p2.time + rSqr[1] * tSqr[2]) * cSqr - sqr(v);
    const double B = 2 * (-rSqr[2] * q_[1] * p1.time + k * (q_[1] * p2.time + q_[2] * p1.time) -
                          rSqr[1] * q_[2] * p2.time) * c;
    double C = rSqr[2] * sqr(q_[1]) - 2 * k * q_[1] * q_[2] + rSqr[1] * sqr(q_[2]);

    double root = NAN;
    if (A == 0) [[unlikely]] {
            if (B == 0) [[unlikely]] {
                    return std::nullopt;
            }
            root = -C / B;
        }
    else {
        double dis = sqr(B) - 4 * A * C;
        if (dis < 0) {
            return std::nullopt;
        } else if (dis == 0)
            [[unlikely]] { root = -B / (2 * A); }
        else {
            double root1 = (-B + sqrt(dis)) / (2 * A);
            double root2 = (-B - sqrt(dis)) / (2 * A);
            if (std::signbit(root1) != std::signbit(root2)) {
                if (std::signbit(root1)) {
                    root = root2;
                } else {
                    root = root1;
                }
            } else {
                return std::nullopt;
            }
        }
    }

    double q[numberPoints - 1] = {0};
    q[0] = q_[1] - root * c * p1.time;
    q[1] = q_[2] - root * c * p2.time;

    return TimePoint<>{(p2.y * q[0] - p1.y * q[1]) / v,
                       (p1.x * q[1] - p2.x * q[0]) / v, -root / c} + p0;
}

std::optional<TimePoint<>> localizationByQPMethod(const std::vector<TimePoint<>>& data,
                                                  double c,
                                                  Combiner& combiner) {
    using FuncType = std::optional<TimePoint<>>(*)(const TimePoint<>&, TimePoint<>, TimePoint<>, double);
    return localization<3, FuncType>(data, c, combiner, localizationByQPMethod);
}

}
