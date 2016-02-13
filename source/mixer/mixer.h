#pragma once

#include <algorithm>
#include <array>
#include <random>

template <class DataType>
class MixerInterface {
public:
    MixerInterface() {}
    MixerInterface(const MixerInterface&) = delete;
    virtual ~MixerInterface() = default;

    virtual void Mix(std::vector<DataType>& deck) = 0;
};

template <class DataType, class RandomEngine>
class RandomMixer : public MixerInterface<DataType> {
public:
    RandomMixer(uint_fast32_t seed) : random_engine_(seed) {}

protected:
    RandomEngine random_engine_;
};
