#include <gtest/gtest.h>

#include <mixer/factory.h>

namespace {
bool DeckChanged(std::shared_ptr<MixerInterface<int>>& mixer,
                 const std::vector<int>& original_deck, size_t attempts) {
    auto deck = original_deck;
    for (size_t i = 0; i != attempts; ++i) {
        mixer->Mix(deck);
        if (deck != original_deck) {
            return true;
        }
    }
    return false;
}
}

TEST(FactoryTest, Default) {
    std::vector<int> test_deck{1, 2, 3, 4};

    MixersFactory factory(test_deck.size());
    auto mixer = factory.CreateMixer<int>(0);

    static const size_t maximum_shuffles = 10;
    bool changed = DeckChanged(mixer, test_deck, maximum_shuffles);
    ASSERT_TRUE(changed) << "Mixer doesn not working";
}

TEST(FactoryTest, OneSwap) {
    std::vector<int> test_deck{1, 2, 3, 4};

    MixersFactory factory(test_deck.size());
    factory.SetMixer(MixersFactory::ONE_SWAP);
    auto mixer = factory.CreateMixer<int>(0);

    static const size_t maximum_shuffles = 10;
    bool changed = DeckChanged(mixer, test_deck, maximum_shuffles);
    ASSERT_TRUE(changed) << "Mixer doesn not working";
}

TEST(FactoryTest, FullCapacity) {
    std::vector<int> test_deck{1, 2, 3, 4};

    MixersFactory factory(test_deck.size());
    factory.SetMixer(MixersFactory::FULL_CAPACITY);
    auto mixer = factory.CreateMixer<int>(0);

    static const size_t maximum_shuffles = 10;
    bool changed = DeckChanged(mixer, test_deck, maximum_shuffles);
    ASSERT_TRUE(changed) << "Mixer doesn not working";
}

TEST(FactoryTest, Ranlux) {
    std::vector<int> test_deck{1, 2, 3, 4};

    MixersFactory factory(test_deck.size());
    factory.SetRandomEngine(MixersFactory::RANLUX24_BASE);
    auto mixer = factory.CreateMixer<int>(0);

    static const size_t maximum_shuffles = 10;
    bool changed = DeckChanged(mixer, test_deck, maximum_shuffles);
    ASSERT_TRUE(changed) << "Mixer doesn not working";
}

TEST(FactoryTest, Minstd) {
    std::vector<int> test_deck{1, 2, 3, 4};

    MixersFactory factory(test_deck.size());
    factory.SetRandomEngine(MixersFactory::MINSTD_RAND);
    auto mixer = factory.CreateMixer<int>(0);

    static const size_t maximum_shuffles = 10;
    bool changed = DeckChanged(mixer, test_deck, maximum_shuffles);
    ASSERT_TRUE(changed) << "Mixer doesn not working";
}

TEST(FactoryTest, WrongMixer) {
    MixersFactory factory(10);
    factory.SetMixer(static_cast<MixersFactory::MixerType>(1000));
    ASSERT_THROW(factory.CreateMixer<int>(0), std::logic_error);
}

TEST(FactoryTest, WrongRandomEngine) {
    MixersFactory factory(10);
    factory.SetRandomEngine(static_cast<MixersFactory::EngineName>(1000));
    ASSERT_THROW(factory.CreateMixer<int>(0), std::logic_error);
}
