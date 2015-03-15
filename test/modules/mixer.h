#ifndef TEST_MIXER_H
#define TEST_MIXER_H

#include "../test_fw.h"
#include <mixer/mixer.h>
#include <functional>

class TestMixer : public TestFW {
public:
    static constexpr std::size_t testSize = 6;

    typedef ::Mixer<int, testSize> Mixer;
    typedef std::array<int, testSize> Deck;

    TestMixer();
protected:
    static logxx::Log cLog;
    bool Tests();

    bool TestStatistics();

    std::size_t CalculateLoops(Mixer&);
    float CalculateAverageLoops(Mixer&);

    std::size_t CalculateDuplicates(Mixer&);
    float CalculateAverageDuplicates(Mixer&);

    static Deck SimpleDeck();
};

#endif /* TEST_MIXER_H */
