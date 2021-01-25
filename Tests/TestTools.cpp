#include "TestTools.h"

namespace pl {
namespace tst {
inline bool eqFloat(double a, double b, double eps) {
    return (isnan(a) || isnan(b)) ? (isnan(a) == isnan(b))
                                  : (fabs(a - b) <= eps);
}

template <class T, class F>
std::optional<std::string> getMessageOfDifferent(const T& real, const T& exp, const std::string& name, F cmpEq) {
    if (!cmpEq(real, exp)) {
        return "Different in " + name + ": " +
               "real: " + std::to_string(real) + "; expected: " + std::to_string(exp) + "\n";
    }
    return {};
}

std::optional<std::string> getMessageOfDifferent(const std::optional<TimePoint<>>& real,
                                                 const std::optional<TimePoint<>>& exp,
                                                 double eps) {
    if (!real.has_value() || !exp.has_value()) {
        if (!real.has_value() && !exp.has_value()) {
            return {};
        }
        return std::string("Real ") +
               (real.has_value() ? "have value, but it doesn't expected" : "haven't value") +
               ".";
    }

    auto cmpEq = [eps](double a, double b){ return eqFloat(a, b, eps); };
    auto ans = getMessageOfDifferent(real->x, exp->x, "x", cmpEq).value_or("") +
               getMessageOfDifferent(real->y, exp->y, "y", cmpEq).value_or("") +
               getMessageOfDifferent(real->time, exp->time, "time", cmpEq).value_or("");
    return ans.empty() ? std::nullopt : std::optional<std::string>(ans);
}

}
}
