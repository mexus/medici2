#include "time_measure.h"

using namespace std::chrono;

TimeMeasure::TimeMeasure() : startPoint(steady_clock::now()) {
}

TimeMeasure::~TimeMeasure() {
}

double TimeMeasure::Elapsed() const {
    duration<double> elapsed = steady_clock::now() - startPoint;
    return elapsed.count();
}
