#ifndef MIXER_FACTORY_H
#define MIXER_FACTORY_H
#include <memory>
#include <cards/standard-36-deck.h>
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

    enum DeckType {
        STANDARD_36_DECK
    };

    std::unique_ptr<MixerInterface<Card, 36>> CreateMixer(std::uint_fast32_t seed)
    {
        switch (deckType) {
            case STANDARD_36_DECK:
                return CreateMixer<Card, 36>(seed);
        }
    }

    template<class T, std::size_t N>
    std::unique_ptr<MixerInterface<T, N>> CreateMixer(std::uint_fast32_t seed)
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

    void SetDeck(DeckType type)
    {
        deckType = type;
    }

private:
    EngineName engine = RANLUX24_BASE;
    MixerType mixerType = FULL_CAPACITY;
    DeckType deckType = STANDARD_36_DECK;

    template<class T, std::size_t N, class RandomEngine>
    std::unique_ptr<MixerInterface<T, N>> CreateMixer(std::uint_fast32_t seed)
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
