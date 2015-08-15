#include "mixer.h"
#include <cmath>
#include <algorithm>
#include <easylogging++.h>

constexpr unsigned long int factorial(unsigned int n) {
    return n <= 1 ? 1 : (n * factorial(n - 1));
}

bool operator<(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    for (std::size_t i = 0; i != TestMixer::test_size; ++i) {
        if (lhs[i] < rhs[i])
            return true;
        else if (lhs[i] > rhs[i])
            return false;
    }
    return false;
}

bool operator==(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    for (std::size_t i = 0; i != TestMixer::test_size; ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& s, const std::vector<int>& deck) {
    for (std::size_t i = 0; i != TestMixer::test_size; ++i) {
        s << deck[i] << " ";
    }
    return s;
}

std::vector<int> TestMixer::SimpleDeck() {
    std::vector<int> deck(test_size);
    for (std::size_t i = 0; i != test_size; ++i) {
        deck[i] = i + 1;
    }
    return deck;
}

TestMixer::TestMixer() : TestFW("mixer") {
}

bool TestMixer::Tests() {
    return TestStatistics("FullCapacityMixer", MixersFactory::FULL_CAPACITY) &&
           TestStatistics("OneSwapMixer", MixersFactory::ONE_SWAP);
}

bool TestMixer::TestStatistics(const std::string& mixer_name,
                               MixersFactory::MixerType mixer_type) {
    LOG(INFO) << "Testing mixer " << mixer_name;
    MixersFactory mixers_factory(test_size);
    mixers_factory.SetMixer(mixer_type);
    auto mixer = mixers_factory.CreateMixer<int>(0);
    float relativeEps = 0.03;
    float loopsResult = CalculateAverageLoops(mixer);
    float loopsEtalon = factorial(test_size);
    if (std::fabs(loopsResult - loopsEtalon) / loopsEtalon > relativeEps) {
        LOG(ERROR) << "Got " << loopsResult << " loops, but should be " << loopsEtalon;
        return false;
    } else
        LOG(DEBUG) << "Got " << loopsResult << " loops, should be " << loopsEtalon;

    float duplicatesResult = CalculateAverageDuplicates(mixer) / factorial(test_size);
    float duplicatesEtalon = 0.5;
    if ((duplicatesResult - duplicatesEtalon) / duplicatesEtalon > relativeEps) {
        LOG(ERROR) << "Got " << duplicatesResult
                   << " duplicates (relative), but should be " << duplicatesEtalon;
        return false;
    } else
        LOG(DEBUG) << "Got " << duplicatesResult << " duplicates (relative), should be "
                   << duplicatesEtalon;

    return true;
}

std::size_t TestMixer::CalculateLoops(Mixer& mixer) {
    auto array = SimpleDeck();
    std::size_t loops = 0;
    auto original = array;
    do {
        mixer->Mix(array);
        ++loops;
    } while (original != array);
    return loops;
}

float TestMixer::CalculateAverageLoops(Mixer& mixer) {
    static const std::size_t tests = 1E4;
    return CalculateAverage(
        tests, [this, &mixer]() -> std::size_t { return CalculateLoops(mixer); });
}

std::size_t TestMixer::CalculateDuplicates(Mixer& mixer) {
    constexpr std::size_t tests = factorial(test_size);
    std::array<std::vector<int>, tests> results;

    results[0] = SimpleDeck();
    for (std::size_t i = 1; i != tests; ++i) {
        results[i] = results[i - 1];
        mixer->Mix(results[i]);
    }

    std::sort(results.begin(), results.end());
    std::size_t duplicates(0);
    int i(0);
    while (i != tests) {
        auto& current = results[i];
        while (++i != tests) {
            auto& test = results[i];
            if (test != current)
                break;
            ++duplicates;
        }
    }

    return duplicates;
}

float TestMixer::CalculateAverageDuplicates(Mixer& mixer) {
    static const std::size_t tests = 1E3;
    return CalculateAverage(
        tests, [this, &mixer]() -> std::size_t { return CalculateDuplicates(mixer); });
}

float TestMixer::CalculateAverage(std::size_t tests,
                                  const std::function<std::size_t()>& f) {
    float res(0.0f);
    for (std::size_t i = 0; i != tests; ++i) {
        res += f();
    }
    return res / tests;
}
