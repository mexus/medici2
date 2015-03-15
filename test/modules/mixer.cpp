#include "mixer.h"
#include <cmath>
#include <algorithm>

constexpr unsigned long int factorial(unsigned int n){
    return n <= 1 ? 1 : (n * factorial(n-1));
}

bool operator<(const TestMixer::Deck& lhs, const TestMixer::Deck& rhs){
    for (std::size_t i = 0; i != TestMixer::testSize; ++i){
        if (lhs[i] < rhs[i])
            return true;
        else if (lhs[i] > rhs[i])
            return false;
    }
    return false;
}

bool operator==(const TestMixer::Deck& lhs, const TestMixer::Deck& rhs){
    for (std::size_t i = 0; i != TestMixer::testSize; ++i){
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& s, const TestMixer::Deck& deck){
    for (std::size_t i = 0; i != TestMixer::testSize; ++i){
        s << deck[i] << " ";
    }
    return s;
}

TestMixer::Deck TestMixer::SimpleDeck() {
    TestMixer::Deck deck;
    for (std::size_t i = 0; i != testSize; ++i){
        deck[i] = i+1;
    }
    return deck;
}

logxx::Log TestMixer::cLog("TestMixer");

TestMixer::TestMixer() : TestFW("mixer", logxx::debug)
{
}

bool TestMixer::Tests(){
    return TestStatistics();
}

bool TestMixer::TestStatistics(){
    S_LOG("TestStatistics");
    Mixer mixer;
    float relativeEps = 0.01;
    float loopsResult = CalculateAverageLoops(mixer);
    float loopsEtalon = factorial(testSize);
    if (std::fabs(loopsResult - loopsEtalon) / loopsEtalon > relativeEps){
        log(logxx::error) << "Got " << loopsResult << " loops, but should be " << loopsEtalon << logxx::endl;
        return false;
    } else
        log(logxx::debug) << "Got " << loopsResult << " loops, should be " << loopsEtalon << logxx::endl;

    float duplicatesResult = CalculateAverageDuplicates(mixer) / factorial(testSize);
    float duplicatesEtalon = 0.5;
    if ((duplicatesResult - duplicatesEtalon) / duplicatesEtalon > relativeEps){
        log(logxx::error) << "Got " << duplicatesResult << " duplicates (relative), but should be " << duplicatesEtalon << logxx::endl;
        return false;
    } else
        log(logxx::debug) << "Got " << duplicatesResult << " duplicates (relative), should be " << duplicatesEtalon << logxx::endl;

    return true;
}

std::size_t TestMixer::CalculateLoops(Mixer& mixer){
    Deck array(SimpleDeck());
    std::size_t loops = 0;
    auto original = array;
    do {
        mixer.Mix(array);
        ++loops;
    } while (original != array);
    return loops;
}

float TestMixer::CalculateAverageLoops(Mixer& mixer){
    static const std::size_t tests = 1E4;
    return CalculateAverage<std::size_t>(tests, [this, &mixer]()->std::size_t{
            return CalculateLoops(mixer);
            });
}

std::size_t TestMixer::CalculateDuplicates(Mixer& mixer){
    Deck array(SimpleDeck());
    constexpr std::size_t tests = factorial(testSize);
    std::array<Deck, tests> results;

    results[0] = array;
    for (std::size_t i = 1; i != tests; ++i){
        results[i] = results[i-1];
        mixer.Mix(results[i]);
    }

    std::sort(results.begin(), results.end());
    std::size_t duplicates(0);
    int i(0);
    while (i != tests){
        auto &current = results[i];
        while (++i != tests){
            auto &test = results[i];
            if (test != current)
                break;
            ++duplicates;
        }
    }

    return duplicates;
}

float TestMixer::CalculateAverageDuplicates(Mixer& mixer){
    static const std::size_t tests = 1E3;
    return CalculateAverage<std::size_t>(tests, [this, &mixer]()->std::size_t{
            return CalculateDuplicates(mixer);
            });
}

