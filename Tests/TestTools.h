#ifndef LDTC_TESTTOOLS_H
#define LDTC_TESTTOOLS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <optional>
#include <string>

#include "Structures/Geometric/Points/TimePoint.h"

#define LOCALIZATION_RESULT_EQ(A, B)                              \
    {                                                             \
        auto result = pl::tst::getMessageOfDifferent(A, B, 1e-7); \
        if (result.has_value()) {                                 \
            FAIL() << result.value();                             \
        }                                                         \
    }

namespace pl {
namespace tst {
std::optional<std::string> getMessageOfDifferent(const std::optional<TimePoint<>>& real,
                                                 const std::optional<TimePoint<>>& exp,
                                                 double eps);

}
}

#endif  // LDTC_TESTTOOLS_H
