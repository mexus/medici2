#pragma once

#include <memory>

#include <cards/sequence.h>
#include <mixer/factory.h>
#include <mixer/mixer.h>

namespace helpers {

struct SwapDereference {
    void operator()(std::vector<cards::Card*>& data, size_t from, size_t to);
};

class SequenceMixer {
public:
    SequenceMixer(
        const cards::Sequence& sequence, cards::Storage storage,
        const std::shared_ptr<MixerInterface<cards::Card*, SwapDereference>>& mixer);

    void Mix();
    const std::vector<cards::Card>& GetDeck() const;

private:
    const cards::Sequence sequence_;
    std::shared_ptr<MixerInterface<cards::Card*, SwapDereference>> mixer_;
    std::vector<cards::Card> deck_;
    std::vector<cards::Card*> mixable_cards_;
};
}
