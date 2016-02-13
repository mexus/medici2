#include <mixer/factory.h>

MixersFactory::MixersFactory(size_t deck_size) : deck_size_(deck_size) {}

void MixersFactory::SetRandomEngine(EngineName engine) {
    engine_ = engine;
}

void MixersFactory::SetMixer(MixerType type) {
    mixer_type_ = type;
}
