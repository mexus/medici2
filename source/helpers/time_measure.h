#ifndef TIME_MEASURE_H
#define	TIME_MEASURE_H

#include <chrono>

class TimeMeasure {
public:
	TimeMeasure();
	virtual ~TimeMeasure();
	
	double Elapsed() const;
private:
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> startPoint;

};

#endif	/* TIME_MEASURE_H */

