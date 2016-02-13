#include "time_measure.h"

using namespace std::chrono;

namespace helpers {

TimeMeasure::TimeMeasure() : start_point_(ClockType::now()) {}

TimeMeasure::~TimeMeasure() {}

double TimeMeasure::SecondsElapsed() const {
    ClockType::duration elapsed = ClockType::now() - start_point_;
    auto elapsed_seconds = duration_cast<duration<double>>(elapsed);
    return elapsed_seconds.count();
}

TimeMeasure::ClockType::rep TimeMeasure::TicksElapsed() const {
    ClockType::duration elapsed = ClockType::now() - start_point_;
    return elapsed.count();
}

}
