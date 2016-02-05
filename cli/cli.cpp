#include "cli.h"
#include <helpers/time_measure.h>
#include <cards/deck-selector.h>
#include <i-ching/i-ching.h>
#include <mixer/factory.h>
#include <easylogging++.h>

using namespace medici;

Cli::Cli(MixersFactory::MixerType mixer_type)
        : mixers_factory_(N), calculator_(mixers_factory_) {
    mixers_factory_.SetMixer(mixer_type);
    calculator_.SetRandomSeeds({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

void Cli::Run() {}
