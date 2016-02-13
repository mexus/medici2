#include <gtest/gtest.h>

#include <thread>
#include <type_traits>

#include <helpers/time_measure.h>

using helpers::TimeMeasure;
using namespace std::chrono;

TEST(TimeMeasure, ClockType) {
    bool is_steady = std::is_same<TimeMeasure::ClockType, steady_clock>::value;
    ASSERT_TRUE(is_steady);
}

TEST(TimeMeasure, Seconds) {
    TimeMeasure measure;

    static const milliseconds sleep_time(10);
    double expected_duration = duration_cast<duration<double>>(sleep_time).count();
    std::this_thread::sleep_for(sleep_time);

    double elapsed = measure.SecondsElapsed();
    ASSERT_NEAR(elapsed, expected_duration, 1e-3);
}

TEST(TimeMeasure, Ticks) {
    TimeMeasure measure;

    static const milliseconds sleep_time(10);
    auto exptected_ticks = duration_cast<TimeMeasure::ClockType::duration>(sleep_time).count();
    std::this_thread::sleep_for(sleep_time);

    auto elapsed_ticks = measure.TicksElapsed();
    ASSERT_NEAR(exptected_ticks, elapsed_ticks, 2e5);
}
