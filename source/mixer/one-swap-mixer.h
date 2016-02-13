#pragma once
#include "mixer.h"

template <class DataType, class RandomEngine>
class OneSwapMixer : public RandomMixer<DataType, RandomEngine> {
public:
    OneSwapMixer(size_t array_size, uint_fast32_t seed)
            : RandomMixer<DataType, RandomEngine>(seed), distribution_(1, array_size - 1) {}

    void Mix(std::vector<DataType>& array) override {
        size_t i = distribution_(this->random_engine_);
        std::swap(array[0], array[i]);
    }

private:
    std::uniform_int_distribution<size_t> distribution_;
};
