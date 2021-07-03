#include "Localization.h"

#include <array>
#include <utility>
#include <functional>
#include <vector>

#include "LocalizationTools.h"

namespace pl {
template<int N>
struct ElderMeadPoint {
public:
    explicit ElderMeadPoint(const std::array<double, N>& data): data_(data) {}
    ElderMeadPoint() {
        for (unsigned int i = 0; i < N; i++) {
            data_[i] = 0;
        }
    }

    double operator[](unsigned int i) const { return data_[i]; }
    double& operator[](unsigned int i) { return data_[i]; }

    ElderMeadPoint<N> operator+(const ElderMeadPoint<N>& b) {
        return combine(*this, b, [](double a, double b) { return a + b; });
    }

    ElderMeadPoint<N> operator-(const ElderMeadPoint<N>& b) {
        return combine(*this, b, [](double a, double b) { return a - b; });
    }

    ElderMeadPoint<N> operator*(double b) {
        ElderMeadPoint<N> result;
        for (unsigned int i = 0; i < N; i++) {
            result[i] = data_[i] * b;
        }
        return result;
    }

    ElderMeadPoint<N> operator/(double b) {
        ElderMeadPoint<N> result;
        for (unsigned int i = 0; i < N; i++) {
            result[i] = data_[i] / b;
        }
        return result;
    }

protected:
    std::array<double, N> data_;

    static ElderMeadPoint<N> combine(const ElderMeadPoint<N>& a,
                                     const ElderMeadPoint<N>& b,
                                     double (*f)(double, double)) {
        ElderMeadPoint<N> result;
        for (unsigned int i = 0; i < N; i++) {
            result[i] = f(a[i], b[i]);
        }
        return result;
    }
};

template<int N>
ElderMeadPoint<N> elderMead(std::function<double (const ElderMeadPoint<N>&)> f,
                            const ElderMeadPoint<N>& start,
                            std::pair<double, unsigned int> breakParameters,
                            unsigned int maxIter,
                            double step,
                            double alpha = 1,
                            double gamma = 2,
                            double rho = -0.5,
                            double sigma = 0.5) {
    std::array<std::pair<ElderMeadPoint<N>, double>, N + 1> points;
    points[0].first = start;
    points[0].second = f(start);
    for (unsigned int i = 1; i <= N; i++) {
        points[i].first = start;
        points[i].first[i - 1] += step;
        points[i].second = f(points[i].first);
    }

    double lastBest = points[0].second;
    unsigned int iterationWithoutImprove = 0;

    for (unsigned int i = 0; i < maxIter; i++) {
        std::sort(points.begin(),
                  points.end(),
                  [](std::pair<ElderMeadPoint<N>, double>& a,
                     std::pair<ElderMeadPoint<N>, double>& b){ return a.second < b.second; });

        if (abs(points[0].second) < abs(lastBest - breakParameters.first)) {
            iterationWithoutImprove = 0;
            lastBest = points[0].second;
        } else {
            if ((++iterationWithoutImprove) >= breakParameters.second) {
                return points[0].first;
            }
        }

        ElderMeadPoint<N> x0;
        for (unsigned int j = 0; j < N; j++) {
            x0 = x0 + points[j].first;
        }
        x0 = x0 / N;

        auto xr = x0 + (x0 - points.back().first) * alpha;
        auto rScore = f(xr);
        if ((points[0].second <= rScore) && (rScore < points[N - 1].second)) {
            points.back() = {xr, rScore};
            continue;
        }

        if (rScore < points[0].second) {
            auto xe = x0 + (x0 - points.back().first) * gamma;
            auto eScore = f(xe);
            if (eScore < rScore) {
                points.back() = {xe, eScore};
            } else {
                points.back() = {xr, rScore};
            }
            continue;
        }

        auto xc = x0 + (x0 - points.back().first) * rho;
        auto cScore = f(xc);
        if (cScore < points.back().second) {
            points.back() = {xc, cScore};
        }

        auto x1 = points[0].first;
        for (unsigned int j = 0; j < points.size(); j++) {
            auto redx = x1 + (points[j].first - x1) * sigma;
            points[j] = { redx, f(redx) };
        }
    }

    return points[0].first;
}

template<int N>
ElderMeadPoint<N> toElderMeadPoint(const TimePoint<>& p) {
    return ElderMeadPoint<N>({p.x, p.y, p.time});
}

template<typename T>
std::optional<TimePoint<>> commonLocalizationByElderMeadMethod(const std::vector<T>& data,
                                                               double c,
                                                               const T& start,
                                                               std::pair<double, unsigned int> breakParameters,
                                                               unsigned int maxNumberIteration,
                                                               double step,
                                                               double alpha,
                                                               double gamma,
                                                               double rho,
                                                               double sigma) {
    const int N = 3;

    auto func = [c, &data](const ElderMeadPoint<3>& p) {
        auto lightning = TimePoint<>(p[0], p[1], p[2]);
        double result = 0;

        for (const auto& detector: data) {
            double delta = dist(detector, lightning) / c - (detector.time - lightning.time);
            result += delta * delta;
        }

        return result;
    };

    auto result = elderMead<N>(func,
                               toElderMeadPoint<N>(start),
                               breakParameters,
                               maxNumberIteration,
                               step,
                               alpha,
                               gamma,
                               rho,
                               sigma);

    for (const auto& i: data) {
        if (result[2] > i.time) {
            return std::nullopt;
        }
    }

    return T(result[0], result[1], result[2]);
}

std::optional<TimePoint<>> localizationByElderMeadMethod(const std::vector<TimePoint<>>& data,
                                                         double c,
                                                         const TimePoint<>& start,
                                                         unsigned int maxNumberIteration,
                                                         std::pair<double, unsigned int> breakParameters,
                                                         double step,
                                                         double alpha,
                                                         double gamma,
                                                         double rho,
                                                         double sigma) {
    return commonLocalizationByElderMeadMethod<TimePoint<>>(data,
                                                            c,
                                                            start,
                                                            breakParameters,
                                                            maxNumberIteration,
                                                            step,
                                                            alpha,
                                                            gamma,
                                                            rho,
                                                            sigma);
}

}
