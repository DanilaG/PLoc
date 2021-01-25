#include "Localization.h"

#include "LocalizationTools.h"

namespace pl {
std::optional<TimePoint<>> localizationByDirectMethod(const TimePoint<>& p0, TimePoint<> p1, TimePoint<> p2, double c) {
    /* Local vars names according to the formula in the documentation. */
    p1.x = p1.x - p0.x;
    p1.y = p1.y - p0.y;
    p2.x = p2.x - p0.x;
    p2.y = p2.y - p0.y;
    double r[3] = {0, dist(p1), dist(p2)};
    if (r[1] == 0) {
        return std::nullopt;
    }
    const double cosA = p1.y / r[1];
    const double sinA = p1.x / r[1];
    p1 = {p1.x * cosA - p1.y * sinA, p1.x * sinA + p1.y * cosA, p1.time};
    p2 = {p2.x * cosA - p2.y * sinA, p2.x * sinA + p2.y * cosA, p2.time};

    if (p1.y == 0 || p2.x == 0) {
        return std::nullopt;
    }
    const double v = p2.y / p1.y;
    const double y1Sqr = sqr(p1.y);
    const double r2Sqr = sqr(r[2]);
    const double x2Sqr = sqr(p2.x);
    const double t0Sqr = sqr(p0.time);
    const double y2Y1 = p2.y * p1.y;
    const double r2SqrDivY1Sqr = r2Sqr / y1Sqr;
    const double cSqr = sqr(c);

    const double d[3] = {0, p1.time - p0.time, p2.time - p0.time};
    const double q[3] = {0, sqr(p1.time) - t0Sqr, sqr(p2.time) - t0Sqr};

    const double A = 4 * cSqr * (cSqr * (d[2] * (d[2] - 2 * v * d[1]) + r2SqrDivY1Sqr * sqr(d[1])) - x2Sqr);
    const double B = 4 * cSqr * (d[2] * (cSqr * (v * q[1] - q[2]) - y2Y1 + r2Sqr) + 2 * x2Sqr * p0.time +
                     d[1] * (v * cSqr * q[2] + r2Sqr * (1 - cSqr * q[1] / y1Sqr - v)));
    const double C = cSqr * (2 * (q[2] * (y2Y1 - r2Sqr) - 2 * x2Sqr * t0Sqr + q[1] *
                                                                              (r2Sqr * (v - 1) - v * cSqr * q[2])) +
                             cSqr * (r2SqrDivY1Sqr * sqr(q[1]) + sqr(q[2]))) + r2Sqr * (p1.y * (p1.y - 2 * p2.y) + r2Sqr);

    double root = 0;
    const double min_time = std::min(p0.time, std::min(p1.time, p2.time));
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
            if (std::signbit(root1 - min_time) !=
                std::signbit(root2 - min_time)) {
                if (std::signbit(root1 - min_time)) {
                    root = root1;
                } else {
                    root = root2;
                }
            } else {
                return std::nullopt;
            }
        }
    }

    const double t1OffsetSqr = sqr(p0.time - root);
    const double y = (y1Sqr - cSqr * (sqr(p1.time - root) - t1OffsetSqr)) / (2 * p1.y);
    Point<> ans = {(r2Sqr - 2 * p2.y * y - cSqr * (sqr(p2.time - root) - t1OffsetSqr)) / (2 * p2.x), y};
    return TimePoint<>{ans.x * cosA + ans.y * sinA + p0.x, -ans.x * sinA + ans.y * cosA + p0.y, root};
}

std::optional<TimePoint<>> localizationByDirectMethod(const std::vector<TimePoint<>>& data,
                                                      double c,
                                                      std::unique_ptr<Combiner> combiner) {
    using FuncType = std::optional<TimePoint<>>(*)(const TimePoint<>&, TimePoint<>, TimePoint<>, double);
    return localization<3, FuncType>(data, c, std::move(combiner), localizationByDirectMethod);
}

}
