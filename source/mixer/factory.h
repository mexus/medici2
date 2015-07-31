#ifndef MIXER_FACTORY_H
#define MIXER_FACTORY_H
#include <memory>
#include "mixer.h"

class MixersFactory {
public:
    MixersFactory(std::size_t deck_size) : deck_size(deck_size) {}

    MixersFactory(const MixersFactory&) = delete;

    enum MixerType { ONE_SWAP, FULL_CAPACITY };

    enum EngineName { RANLUX24_BASE, MINSTD_RAND };

    template <class T>
    std::unique_ptr<MixerInterface<T>> CreateMixer(std::uint_fast32_t seed) const {
        switch (engine) {
            case RANLUX24_BASE:
                return CreateMixer<T, std::ranlux24_base>(seed);
            case MINSTD_RAND:
                return CreateMixer<T, std::minstd_rand>(seed);
            default:
                throw std::logic_error("Unimplemented engine " + std::to_string(engine));
        }
    }

    void SetRandomEngine(EngineName engine) { this->engine = engine; }

    void SetMixer(MixerType type) { mixer_type = type; }

private:
    const std::size_t deck_size;
    EngineName engine = RANLUX24_BASE;
    MixerType mixer_type = FULL_CAPACITY;

    template <class T, class RandomEngine>
    std::unique_ptr<MixerInterface<T>> CreateMixer(std::uint_fast32_t seed) const {
        switch (mixer_type) {
            case ONE_SWAP:
                return std::unique_ptr<MixerInterface<T>>(
                    new OneSwapMixer<T, RandomEngine>(deck_size, seed));
            case FULL_CAPACITY:
                return std::unique_ptr<MixerInterface<T>>(
                    new FullCapacityMixer<T, RandomEngine>(deck_size, seed));
            default:
                throw std::logic_error("Unimplemented mixer type " +
                                       std::to_string(mixer_type));
        }
    }
};

#endif /* MIXER_FACTORY_H */
