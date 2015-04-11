#ifndef MIXER_H
#define MIXER_H

#include <random>
#include <array>
#include <algorithm>

template<class T, std::size_t N, class RandomEnging = std::minstd_rand>
class Mixer {
public:
    Mixer(uint_fast32_t seed = 0) : randomEngine(seed), uniformDistribution(1, N - 1) {}
    Mixer(const Mixer&) = delete;

    void Mix(std::array<T, N>& array) {
        std::size_t i = uniformDistribution(randomEngine);
        std::swap(array[0], array[i]);
    }
    void Shuffle(std::array<T, N>& array) {
        std::shuffle(array.begin(), array.end(), randomEngine);
    }
private:
    RandomEnging randomEngine;
    std::uniform_int_distribution<std::size_t> uniformDistribution;
};

#endif /* MIXER_H */
