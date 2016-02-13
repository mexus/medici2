#include <gtest/gtest.h>

#include <mixer/full-capacity-mixer.h>
#include <mixer/one-swap-mixer.h>

namespace {
static const std::set<std::vector<int>> all_combinations{{1, 2, 3}, {1, 3, 2}, {2, 1, 3},
                                                         {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};

static const size_t deck_size = all_combinations.begin()->size();

std::set<std::vector<int>> GenerateCombinations(MixerInterface<int>* mixer,
                                                size_t maximum_shuffles) {
    auto deck = *all_combinations.begin();
    std::set<std::vector<int>> generated_combinations;
    for (size_t i = 0; i != maximum_shuffles; ++i) {
        mixer->Mix(deck);
        generated_combinations.insert(deck);
    }
    return generated_combinations;
}
}

TEST(MixersTest, OneSwap) {
    OneSwapMixer<int, std::ranlux24_base> mixer(deck_size, 0);

    static const size_t maximum_shuffles = 19;
    auto generated_combinations = GenerateCombinations(&mixer, maximum_shuffles);

    ASSERT_EQ(generated_combinations, all_combinations);
}

TEST(MixersTest, FullCapacity) {
    FullCapacityMixer<int, std::ranlux24_base> mixer(deck_size, 0);

    static const size_t maximum_shuffles = 22;
    auto generated_combinations = GenerateCombinations(&mixer, maximum_shuffles);

    ASSERT_EQ(generated_combinations, all_combinations);
}
