#include "time_measure.h"

TimeMeasure::TimeMeasure() : startPoint(std::chrono::steady_clock::now())
{
}

TimeMeasure::~TimeMeasure()
{
}

double TimeMeasure::Elapsed() const{
	std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - startPoint;
	return elapsed.count();
}
