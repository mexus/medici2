#ifndef MIXER_MIXER_H
#define MIXER_MIXER_H

#include <random>
#include <array>
#include <algorithm>

template<class T, std::size_t N>
class MixerInterface {
public:
    MixerInterface() = default;
    virtual ~MixerInterface() = default;
    MixerInterface(const MixerInterface&) = delete;
    virtual void Mix(std::array<T, N>& array) = 0;
};

template<class T, std::size_t N, class RandomEngine>
class RandomMixer : public MixerInterface<T, N> {
public:
    RandomMixer(uint_fast32_t seed) :
        randomEngine(seed)
    {
    }
protected:
    RandomEngine randomEngine;
};

template<class T, std::size_t N, class RandomEngine = std::ranlux24_base>
class OneSwapMixer : public RandomMixer<T, N, RandomEngine> {
public:
    OneSwapMixer(uint_fast32_t seed) :
        RandomMixer<T, N, RandomEngine>(seed),
        uniformDistribution(1, N - 1)
    {
    }

    void Mix(std::array<T, N>& array) override
    {
        std::size_t i = uniformDistribution(this->randomEngine);
        std::swap(array[0], array[i]);
    }
private:
    std::uniform_int_distribution<std::size_t> uniformDistribution;
};

template<class T, std::size_t N, class RandomEngine = std::ranlux24_base>
class FullCapacityMixer : public RandomMixer<T, N, RandomEngine> {
public:
    FullCapacityMixer(uint_fast32_t seed) :
        RandomMixer<T, N, RandomEngine>(seed)
    {
    }

    void Mix(std::array<T, N>& array) override
    {
        auto rand = this->randomEngine();
        for (std::size_t i = 0; i + 1 != N && rand != 0; ++i) {
            std::size_t rank = N - i;
            std::size_t swapWithShift = rand % rank;
            if (swapWithShift != 0)
                std::swap(array[i], array[i + swapWithShift]);
            rand /= rank;
        }
    }
};

#endif /* MIXER_MIXER_H */
