#ifndef PLOC_LOCALIZATIONTOOLS_H
#define PLOC_LOCALIZATIONTOOLS_H

#include <array>
#include <vector>

#include "Combiner/Combiner.h"
#include "Geometric/Points/TimePoint.h"

namespace pl {
template <class T>
T sqr(const T& a) {
    return a * a;
}

template<unsigned int K>
class CombinationsWithoutRepetition {
public:
    CombinationsWithoutRepetition(unsigned int n) : n_(n) {
        for (unsigned int i = 0; i < K; i++) {
            combination_[i] = i;
        }
    }

    const std::array<unsigned int, K> &get() const { return combination_; }

    bool next() {
        increase(K - 1);
        return !isEnd();
    }

    bool isEnd() { return isEnd_; }

private:
    unsigned int n_;
    std::array<unsigned int, K> combination_ = {};
    bool isEnd_ = false;

    void increase(int index) {
        if (index < 0) {
            isEnd_ = true;
            return;
        }

        if (combination_[index] >= (n_ - 1) - (K - 1) + index) {
            increase(index - 1);
            return;
        }

        combination_[index]++;
        for (unsigned int i = index + 1; i < K; i++) {
            combination_[i] = combination_[index] + (i - index);
        }
    }
};

template<class F, unsigned int K, class T, T... ints>
std::optional<TimePoint<>> executeLocalization(const std::vector<TimePoint<>>& data,
                                double c,
                                F funcOfLocalization,
                                const std::array<unsigned int, K>& combination,
                                std::integer_sequence<T, ints...>) {
    return funcOfLocalization((data[combination[ints]])..., c);
}

template <unsigned int K, class F>
std::optional<TimePoint<>> localization(const std::vector<TimePoint<>>& data,
                         double c,
                         std::unique_ptr<Combiner> combiner,
                         F funcOfLocalization) {
    if (data.size() < K) {
        return std::nullopt;
    }

    CombinationsWithoutRepetition<K> combinations(data.size());
    auto indexSequence = std::make_index_sequence<K>{};
    combiner->reset();

    do {
        auto combination = combinations.get();
        auto result = executeLocalization<F, K, typename decltype(indexSequence)::value_type>(data, c, funcOfLocalization, combination, indexSequence);
        if (result.has_value()) {
            combiner->add(result.value());
        }
    } while(combinations.next());

    return combiner->result();
}

}

#endif  // PLOC_LOCALIZATIONTOOLS_H
