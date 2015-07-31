#ifndef MIXER_MIXER_H
#define MIXER_MIXER_H

#include <random>
#include <array>
#include <algorithm>

template <class T>
class MixerInterface {
public:
    virtual void Mix(std::vector<T>& deck) = 0;
    virtual ~MixerInterface() = default;

protected:
    MixerInterface(std::size_t array_size) : array_size(array_size) {}

    MixerInterface(const MixerInterface&) = delete;

    const std::size_t array_size;
};

template <class T, class RandomEngine>
class RandomMixer : public MixerInterface<T> {
protected:
    RandomMixer(std::size_t array_size, uint_fast32_t seed)
            : MixerInterface<T>(array_size), randomEngine(seed) {}

    RandomEngine randomEngine;
};

template <class T, class RandomEngine = std::ranlux24_base>
class OneSwapMixer : public RandomMixer<T, RandomEngine> {
protected:
    OneSwapMixer(std::size_t array_size, uint_fast32_t seed)
            : RandomMixer<T, RandomEngine>(array_size, seed),
              distribution(1, array_size - 1) {}

    std::uniform_int_distribution<std::size_t> distribution;
    friend class MixersFactory;

public:
    void Mix(std::vector<T>& array) override {
        std::size_t i = distribution(this->randomEngine);
        std::swap(array[0], array[i]);
    }
};

template <class T, class RandomEngine = std::ranlux24_base>
class FullCapacityMixer : public RandomMixer<T, RandomEngine> {
protected:
    FullCapacityMixer(std::size_t array_size, uint_fast32_t seed)
            : RandomMixer<T, RandomEngine>(array_size, seed) {}

    friend class MixersFactory;

public:
    void Mix(std::vector<T>& array) override {
        std::size_t N = MixerInterface<T>::array_size;
        auto random_value = this->randomEngine();
        for (std::size_t i = 0; i + 1 != N && random_value != 0; ++i) {
            std::size_t rank = N - i;
            std::size_t swapWithShift = random_value % rank;
            if (swapWithShift != 0)
                std::swap(array[i], array[i + swapWithShift]);
            random_value /= rank;
        }
    }
};

#endif /* MIXER_MIXER_H */
