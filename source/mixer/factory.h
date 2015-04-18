#ifndef MIXER_FACTORY_H
#define MIXER_FACTORY_H
#include <memory>
#include "mixer.h"

class MixersFactory {
public:
    MixersFactory() = default;
    MixersFactory(const MixersFactory&) = delete;

    enum MixerType {
        ONE_SWAP,
        FULL_CAPACITY
    };

    enum EngineName {
        RANLUX24_BASE,
        MINSTD_RAND
    };

    template<class T, std::size_t N>
    std::unique_ptr<MixerInterface<T, N>> CreateMixer(std::uint_fast32_t seed) const
    {
        switch (engine) {
            case RANLUX24_BASE:
                return CreateMixer<T, N, std::ranlux24_base>(seed);
            case MINSTD_RAND:
                return CreateMixer<T, N, std::minstd_rand>(seed);
        }
    }

    void SetRandomEngine(EngineName engine)
    {
        this->engine = engine;
    }

    void SetMixer(MixerType type)
    {
        mixerType = type;
    }

private:
    EngineName engine = RANLUX24_BASE;
    MixerType mixerType = FULL_CAPACITY;

    template<class T, std::size_t N, class RandomEngine>
    std::unique_ptr<MixerInterface<T, N>> CreateMixer(std::uint_fast32_t seed) const
    {
        switch (mixerType) {
            case ONE_SWAP:
                return std::unique_ptr<MixerInterface<T, N>>(new OneSwapMixer<T, N, RandomEngine>(seed));
            case FULL_CAPACITY:
                return std::unique_ptr<MixerInterface<T, N>>(new FullCapacityMixer<T, N, RandomEngine>(seed));
        }
    }
};

#endif /* MIXER_FACTORY_H */
