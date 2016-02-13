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

    template <class DataType>
    std::unique_ptr<MixerInterface<DataType>> CreateMixer(uint_fast32_t seed) const {
        switch (engine_) {
            case RANLUX24_BASE:
                return CreateMixer<DataType, std::ranlux24_base>(seed);
            case MINSTD_RAND:
                return CreateMixer<DataType, std::minstd_rand>(seed);
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

    template <class DataType, class RandomEngine>
    std::unique_ptr<MixerInterface<DataType>> CreateMixer(uint_fast32_t seed) const {
        switch (mixer_type_) {
            case ONE_SWAP:
                return std::unique_ptr<MixerInterface<DataType>>(
                    new OneSwapMixer<DataType, RandomEngine>(deck_size_, seed));
            case FULL_CAPACITY:
                return std::unique_ptr<MixerInterface<DataType>>(
                    new FullCapacityMixer<DataType, RandomEngine>(deck_size_, seed));
            default:
                throw std::logic_error("Unimplemented mixer type " +
                                       std::to_string(mixer_type_));
        }
    }
};
