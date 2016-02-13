#pragma once
#include <chrono>

namespace helpers {

class TimeMeasure {
public:
    using ClockType = std::chrono::steady_clock;

    TimeMeasure();
    virtual ~TimeMeasure();

    double SecondsElapsed() const;
    ClockType::rep TicksElapsed() const;

private:

    const ClockType::time_point start_point_;
};

}
