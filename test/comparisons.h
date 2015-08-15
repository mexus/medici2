#ifndef TEST_COMPARISONS_H
#define TEST_COMPARISONS_H

#include <vector>
#include <set>
#include <map>
#include <easylogging++.h>

template <class T>
bool Compare(const std::set<T>& etalon, const std::set<T>& test) {
    bool res = true;
    for (auto el : etalon) {
        auto itTest = test.find(el);
        if (itTest == test.end()) {
            LOG(ERROR) << "Element {" << el << "} not found in a test set";
            res = false;
        }
    }
    for (auto el : test) {
        auto itEtalon = etalon.find(el);
        if (itEtalon == etalon.end()) {
            LOG(ERROR) << "Extra element {" << el << "} in a test set";
            res = false;
        }
    }
    return res;
}

template <class T1, class T2>
bool Compare(const std::map<T1, T2>& etalon, const std::map<T1, T2>& test) {
    bool res = true;
    for (auto pair : etalon) {
        auto& el = pair.first;
        auto itTest = test.find(el);
        if (itTest == test.end()) {
            LOG(ERROR) << "Element {" << el << "} not found in a test set";
            res = false;
        } else if (!(pair.second == itTest->second)) {
            LOG(ERROR) << "Element {" << el << "} has value {" << pair.second
                       << "} in etalon, but {" << itTest->second << "} in a test set";
            res = false;
        }
    }

    for (auto pair : test) {
        auto& el = pair.first;
        auto itEtalon = etalon.find(el);
        if (itEtalon == etalon.end()) {
            LOG(ERROR) << "Extra element {" << el << "} in a test set";
            res = false;
        } else if (!(pair.second == itEtalon->second)) {
            LOG(ERROR) << "Element {" << el << "} has value {" << itEtalon->second
                       << "} in etalon, but {" << pair.second << "} in a test set";
            res = false;
        }
    }
    return res;
}

template <class T, std::size_t N>
bool Compare(const std::array<T, N>& etalon, const std::array<T, N>& test) {
    bool res = true;
    for (std::size_t i = 0; i != N; ++i) {
        auto& etalonEl = etalon[i];
        auto& testEl = test[i];
        if (!(etalonEl == testEl)) {
            LOG(ERROR) << "Error at element #" << i << ": got {" << testEl
                       << "}, but should be {" << etalonEl << "}";
            res = false;
        }
    }
    return res;
}

#endif /* TEST_COMPARISONS_H */
