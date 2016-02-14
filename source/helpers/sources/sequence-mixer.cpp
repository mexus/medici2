#include <helpers/sequence-mixer.h>

#include <cassert>

#include <cards/storage.h>

namespace helpers {

void SwapDereference::operator()(std::vector<cards::Card*>& data, size_t from,
                                 size_t to) {
    std::swap(*data[from], *data[to]);
}

SequenceMixer::SequenceMixer(
    const cards::Sequence& sequence, cards::Storage storage,
    const std::shared_ptr<MixerInterface<cards::Card*, SwapDereference>>& mixer)
        : sequence_(sequence), mixer_(mixer) {
    sequence_.TakeCardsFromStorage(&storage);
    std::vector<size_t> mixable_positions;
    auto available_cards = storage.GetAllAvailableCards();
    size_t position = 0;
    while (!available_cards.empty()) {
        if (sequence.IsOccupied(position)) {
            deck_.push_back(sequence.GetCard(position));
        } else {
            deck_.push_back(available_cards.back());
            available_cards.pop_back();
            mixable_positions.push_back(position);
        }
        ++position;
    }
    assert(!mixable_positions.empty() && "Nothing to mix!");
    for (auto& position : mixable_positions) {
        mixable_cards_.push_back(deck_.data() + position);
    }
}

const std::vector<cards::Card>& SequenceMixer::GetDeck() const {
    return deck_;
}

void SequenceMixer::Mix() {
    if (mixable_cards_.empty()) {
        throw std::logic_error("Nothing to mix :(");
    }
    mixer_->Mix(mixable_cards_);
}
}
