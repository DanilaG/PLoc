#include "Localization.h"

#include "LocalizationTools.h"

namespace pl {
inline double computeDeterminant(const double* a, const double* b, const double* c) {
    return a[0] * b[1] * c[2] + a[1] * b[2] * c[0] + b[0] * c[1] * a[2] -
           c[0] * b[1] * a[2] - a[1] * b[0] * c[2] - b[2] * c[1] * a[0];
}

std::optional<TimePoint<>> localizationByQuadrangleMethod(const TimePoint<>& p0,
                                                          TimePoint<> p1,
                                                          TimePoint<> p2,
                                                          TimePoint<> p3, double c) {
    /* Local vars names according to the formula in the documentation. */
    p1 = p1 - p0;
    p2 = p2 - p0;
    p3 = p3 - p0;

    const double distP1 = dist(p1);
    if (distP1 == 0) {
        return std::nullopt;
    }
    const double cosA = p1.y / distP1;
    const double sinA = p1.x / distP1;
    p1 = {p1.x * cosA - p1.y * sinA, p1.x * sinA + p1.y * cosA, p1.time};
    p2 = {p2.x * cosA - p2.y * sinA, p2.x * sinA + p2.y * cosA, p2.time};
    p3 = {p3.x * cosA - p3.y * sinA, p3.x * sinA + p3.y * cosA, p3.time};

    const double cSqr = sqr(c);

    const double A[3][3] = {
        {0,              -p2.x,          -p3.x},
        {-p1.y,          -p2.y,          -p3.y},
        {p1.time * cSqr, p2.time * cSqr, p3.time * cSqr}};
    const double B[3] = {(sqr(p1.time) * cSqr - sqr(p1.y)) / 2,
                         (sqr(p2.time) * cSqr - sqr(p2.y) - sqr(p2.x)) / 2,
                         (sqr(p3.time) * cSqr - sqr(p3.y) - sqr(p3.x)) / 2};

    const double determinant = computeDeterminant(A[0], A[1], A[2]);
    if (determinant == 0) {
        return std::nullopt;
    }
    TimePoint<> ans = {computeDeterminant(B, A[1], A[2]) / determinant,
                       computeDeterminant(A[0], B, A[2]) / determinant,
                       computeDeterminant(A[0], A[1], B) / determinant};
    return TimePoint<>{ans.x * cosA + ans.y * sinA + p0.x,
                       -ans.x * sinA + ans.y * cosA + p0.y,
                       ans.time + p0.time};
}

std::optional<TimePoint<>> localizationByQuadrangleMethod(const std::vector<TimePoint<>>& data,
                                                          double c,
                                                          Combiner& combiner) {
    using FuncType = std::optional<TimePoint<>>(*)(const TimePoint<> &, TimePoint<>, TimePoint<>, TimePoint<>, double);
    return localization<4, FuncType>(data, c, combiner, localizationByQuadrangleMethod);
}

}
