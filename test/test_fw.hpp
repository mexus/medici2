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

template<class T>
bool TestFW::Compare(const std::set<T>& etalon, const std::set<T>& test){
	static logxx::Log log("CompareSet");
	bool res = true;
	for (auto el : etalon){
		auto itTest = test.find(el);
		if (itTest == test.end()){
			log(logxx::error) << "Element {" << el << "} not found in a test set" << logxx::endl;
			res = false;
		}
	}
	for (auto el : test){
		auto itEtalon = etalon.find(el);
		if (itEtalon == etalon.end()){
			log(logxx::error) << "Extra element {" << el << "} in a test set" << logxx::endl;
			res = false;
		}
	}
	return res;
}

template<class T1, class T2>
bool TestFW::Compare(const std::map<T1, T2>& etalon, const std::map<T1, T2>& test){
	static logxx::Log log("CompareMap");
	bool res = true;
	for (auto pair : etalon){
		auto &el = pair.first;
		auto itTest = test.find(el);
		if (itTest == test.end()){
			log(logxx::error) << "Element {" << el << "} not found in a test set" << logxx::endl;
			res = false;
		} else if (!(pair.second == itTest->second )){
			log(logxx::error) << "Element {" << el << "} has value {" << pair.second << "} in etalon, but {" <<
				itTest->second << "} in a test set" << logxx::endl;
			res = false;
		}
	}

	for (auto pair : test){
		auto &el = pair.first;
		auto itEtalon = etalon.find(el);
		if (itEtalon == etalon.end()){
			log(logxx::error) << "Extra element {" << el << "} in a test set" << logxx::endl;
			res = false;
		} else if (!(pair.second == itEtalon->second )){
			log(logxx::error) << "Element {" << el << "} has value {" << itEtalon->second << "} in etalon, but {" <<
				pair.second  << "} in a test set" << logxx::endl;
			res = false;
		}
	}
	return res;
}

template<class T, std::size_t N>
bool TestFW::Compare(const std::array<T, N>& etalon, const std::array<T, N>& test){
	static logxx::Log log("CompareArray");
	bool res = true;
	for (std::size_t i = 0; i != N; ++i){
		auto &etalonEl = etalon[i];
		auto &testEl = test[i];
		if (!(etalonEl == testEl)){
			log(logxx::error) << "Error at element #" << i << ": got {" << testEl << "}, but should be {" << etalonEl << "}" << logxx::endl;
			res = false;
		}
	}
	return res;
}


#endif /* TEST_FW_HPP */
