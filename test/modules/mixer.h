#ifndef TEST_MIXER_H
#define TEST_MIXER_H

#include <test_fw.h>
#include <mixer/mixer.h>
#include <functional>

class TestMixer : public TestFW {
public:
    static constexpr std::size_t testSize = 6;

    //typedef ::FullCapacityMixer<int, testSize> Mixer;
    typedef std::array<int, testSize> Deck;

    TestMixer();
protected:
    static logxx::Log cLog;
    bool Tests();

    template<class Mixer>
    bool TestStatistics(const std::string& mixerName);

    template<class Mixer>
    std::size_t CalculateLoops(Mixer&);

    template<class Mixer>
    std::size_t CalculateDuplicates(Mixer&);

    template<class Mixer>
    float CalculateAverageLoops(Mixer&);

    template<class Mixer>
    float CalculateAverageDuplicates(Mixer&);

    static Deck SimpleDeck();
    static float CalculateAverage(std::size_t tests, const std::function<std::size_t()>& f);
};

#endif /* TEST_MIXER_H */
