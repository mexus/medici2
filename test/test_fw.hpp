#ifndef TEST_FW_HPP
#define TEST_FW_HPP

#include "test_fw.h"

template<class T, class Result>
Result TestFW::CalculateAverage(std::size_t tests, const std::function<T()>& f){
	Result res(0);
	for (std::size_t i = 0; i != tests; ++i){
		res += f();
	}
	return static_cast<Result>(res) / tests;
}

#endif /* TEST_FW_HPP */
