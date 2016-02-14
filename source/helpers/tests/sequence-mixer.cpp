#include <algorithm>

#include <gtest/gtest.h>

#include <cards/storage.h>
#include <helpers/sequence-mixer.h>

using namespace helpers;
using namespace cards;

namespace {

struct FakeMixer : public MixerInterface<Card*, SwapDereference> {
    void Mix(std::vector<Card*>& deck) override {
        if (!enabled) {
            return;
        }
        if (deck.size() < 2) {
            return;
        }
        this->swap_functor_(deck, 0, 1);
    }

    bool enabled = false;
};
}

TEST(Helpers_SequenceMixer, Constructor) {
    std::vector<Card> cards{{0, 1}, {0, 2}, {0, 3}, {1, 1}, {1, 2}, {1, 3}};
    Sequence sequence(SimpleSequence{{{0, 3}, {0, 1}, {0, 2}}, 1});
    Storage storage(cards);
    auto mixer = std::make_shared<FakeMixer>();

    SequenceMixer sequence_mixer(sequence, storage, mixer);

    ASSERT_EQ(std::vector<Card>({{1, 3}, {0, 3}, {0, 1}, {0, 2}, {1, 2}, {1, 1}}),
              sequence_mixer.GetDeck());
}

TEST(Helpers_SequenceMixer, Mixing) {
    std::vector<Card> cards{{0, 1}, {0, 2}, {0, 3}, {1, 1}, {1, 2}, {1, 3}};
    Sequence sequence(SimpleSequence{{{0, 3}, {0, 1}, {0, 2}}, 1});
    Storage storage(cards);
    auto mixer = std::make_shared<FakeMixer>();

    SequenceMixer sequence_mixer(sequence, storage, mixer);

    sequence_mixer.Mix();
    ASSERT_EQ(std::vector<Card>({{1, 3}, {0, 3}, {0, 1}, {0, 2}, {1, 2}, {1, 1}}),
              sequence_mixer.GetDeck());

    mixer->enabled = true;
    sequence_mixer.Mix();
    ASSERT_EQ(std::vector<Card>({{1, 2}, {0, 3}, {0, 1}, {0, 2}, {1, 3}, {1, 1}}),
              sequence_mixer.GetDeck());
}
