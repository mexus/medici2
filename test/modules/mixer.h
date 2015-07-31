#ifndef TEST_MIXER_H
#define TEST_MIXER_H

#include <test_fw.h>
#include <mixer/factory.h>
#include <functional>

class TestMixer : public TestFW {
public:
    static constexpr std::size_t test_size = 6;
    typedef std::unique_ptr<MixerInterface<int>> Mixer;

    TestMixer();

protected:
    static logxx::Log cLog;
    bool Tests();

    bool TestStatistics(const std::string& mixer_name,
                        MixersFactory::MixerType mixer_type);

    std::size_t CalculateLoops(Mixer&);
    std::size_t CalculateDuplicates(Mixer&);
    float CalculateAverageLoops(Mixer&);
    float CalculateAverageDuplicates(Mixer&);

    static std::vector<int> SimpleDeck();
    static float CalculateAverage(std::size_t tests,
                                  const std::function<std::size_t()>& f);
};

#endif /* TEST_MIXER_H */
