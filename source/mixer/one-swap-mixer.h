#pragma once
#include "mixer.h"

template <class DataType, class RandomEngine, class SwapFunctor = DefaultSwap<DataType>>
class OneSwapMixer : public RandomMixer<DataType, RandomEngine, SwapFunctor> {
public:
    OneSwapMixer(size_t array_size, uint_fast32_t seed, SwapFunctor swap_functor = {})
            : RandomMixer<DataType, RandomEngine, SwapFunctor>(seed, swap_functor),
              distribution_(1, array_size - 1) {}

    void Mix(std::vector<DataType>& array) override {
        size_t i = distribution_(this->random_engine_);
        this->swap_functor_(array, 0, i);
    }

private:
    std::uniform_int_distribution<size_t> distribution_;
};
