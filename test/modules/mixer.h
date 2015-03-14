#ifndef TEST_MIXER_H
#define TEST_MIXER_H

#include "../test_fw.h"
#include <mixer/mixer.h>
#include <functional>

typedef Mixer<int, 5> Int5Mixer;
typedef std::array<int, 5> Int5Deck;

class TestMixer : public TestFW {
public:
    TestMixer();
protected:
    static logxx::Log cLog;
    bool Tests();

    bool TestStatistics();

    std::size_t CalculateLoops(Int5Mixer&);
    float CalculateAverageLoops(Int5Mixer&);

    std::size_t CalculateDuplicates(Int5Mixer&);
    float CalculateAverageDuplicates(Int5Mixer&);

};

#endif /* TEST_MIXER_H */
