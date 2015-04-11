#ifndef MIXER_H
#define MIXER_H

#include <random>
#include <array>
#include <algorithm>

template<class T, std::size_t N, class RandomEngine = std::ranlux24_base>
class Mixer {
public:
    Mixer(uint_fast32_t seed = 0) :
        randomEngine(seed)
    {
    }

    Mixer(const Mixer&) = delete;

    void Mix(std::array<T, N>& array) {
        auto rand = randomEngine();
        for (std::size_t i = 0; i + 1 != N && rand != 0; ++i) {
            std::size_t rank = N - i;
            std::size_t swapWithShift = rand % rank;
            if (swapWithShift != 0)
                std::swap(array[i], array[i + swapWithShift]);
            rand /= rank;
        }
    }
private:
    RandomEngine randomEngine;
};

#endif /* MIXER_H */
