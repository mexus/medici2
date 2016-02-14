#pragma once

#include <algorithm>
#include <array>
#include <random>

template <class DataType>
struct DefaultSwap {
    void operator()(std::vector<DataType>& data, size_t from, size_t to) {
        std::swap(data[from], data[to]);
    }
};

template <class DataType, class SwapFunctor = DefaultSwap<DataType>>
class MixerInterface {
public:
    MixerInterface(SwapFunctor swap_functor = {}) : swap_functor_(swap_functor) {}
    MixerInterface(const MixerInterface&) = delete;
    virtual ~MixerInterface() = default;

    virtual void Mix(std::vector<DataType>& deck) = 0;

protected:
    SwapFunctor swap_functor_;
};

template <class DataType, class RandomEngine, class SwapFunctor = DefaultSwap<DataType>>
class RandomMixer : public MixerInterface<DataType, SwapFunctor> {
public:
    RandomMixer(uint_fast32_t seed, SwapFunctor swap_functor = {})
            : MixerInterface<DataType, SwapFunctor>(swap_functor), random_engine_(seed) {}

protected:
    RandomEngine random_engine_;
};
