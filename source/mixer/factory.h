#pragma once
#include <memory>

#include "full-capacity-mixer.h"
#include "one-swap-mixer.h"

class MixersFactory {
public:
    MixersFactory(size_t deck_size);
    MixersFactory(const MixersFactory& other) = delete;

    enum MixerType { ONE_SWAP, FULL_CAPACITY };
    enum EngineName { RANLUX24_BASE, MINSTD_RAND };

    template <class DataType, class SwapFunctor = DefaultSwap<DataType>>
    std::shared_ptr<MixerInterface<DataType, SwapFunctor>> CreateMixer(
        uint_fast32_t seed, SwapFunctor swap_functor = {}) const {
        switch (engine_) {
            case RANLUX24_BASE:
                return CreateMixer<DataType, std::ranlux24_base, SwapFunctor>(
                    seed, swap_functor);
            case MINSTD_RAND:
                return CreateMixer<DataType, std::minstd_rand, SwapFunctor>(seed,
                                                                            swap_functor);
            default:
                throw std::logic_error("Unimplemented engine " + std::to_string(engine_));
        }
    }

    void SetRandomEngine(EngineName engine);
    void SetMixer(MixerType type);

private:
    const std::size_t deck_size_;
    EngineName engine_ = RANLUX24_BASE;
    MixerType mixer_type_ = FULL_CAPACITY;

    template <class DataType, class RandomEngine, class SwapFunctor>
    std::shared_ptr<MixerInterface<DataType, SwapFunctor>> CreateMixer(
        uint_fast32_t seed, SwapFunctor swap_functor) const {
        switch (mixer_type_) {
            case ONE_SWAP:
                return std::shared_ptr<MixerInterface<DataType, SwapFunctor>>(
                    new OneSwapMixer<DataType, RandomEngine, SwapFunctor>(
                        deck_size_, seed, swap_functor));
            case FULL_CAPACITY:
                return std::shared_ptr<MixerInterface<DataType, SwapFunctor>>(
                    new FullCapacityMixer<DataType, RandomEngine, SwapFunctor>(
                        deck_size_, seed, swap_functor));
            default:
                throw std::logic_error("Unimplemented mixer type " +
                                       std::to_string(mixer_type_));
        }
    }
};
