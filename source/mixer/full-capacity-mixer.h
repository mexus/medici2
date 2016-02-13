#pragma once
#include "mixer.h"

template <class T, class RandomEngine>
class FullCapacityMixer : public RandomMixer<T, RandomEngine> {
public:
    FullCapacityMixer(size_t array_size, uint_fast32_t seed)
            : RandomMixer<T, RandomEngine>(seed), array_size_(array_size) {}

    void Mix(std::vector<T>& array) override {
        size_t N = array_size_;
        auto random_value = this->random_engine_();
        for (size_t i = 0; i + 1 != N && random_value != 0; ++i) {
            size_t rank = N - i;
            size_t swap_with_shift = random_value % rank;
            if (swap_with_shift != 0)
                std::swap(array[i], array[i + swap_with_shift]);
            random_value /= rank;
        }
    }

private:
    const size_t array_size_;
};
