#ifndef TEST_OPERATORS_HPP
#define TEST_OPERATORS_HPP

#include <vector>
#include <set>
#include <ostream>

template<class T>
std::ostream& PrintVector(std::ostream& s, const T& vec){
	bool first(true);
	for (auto &el : vec){
		if (first) first = false; else s << ", ";
		s << el;
	}
	return s;
}

template<class T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& vec){
	return PrintVector(s, vec);
}

template<class T>
std::ostream& operator<<(std::ostream& s, const std::set<T>& vec){
	return PrintVector(s, vec);
}

#endif /* TEST_OPERATORS_HPP */
