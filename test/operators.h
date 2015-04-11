#ifndef TEST_OPERATORS_H
#define TEST_OPERATORS_H

#include <vector>
#include <set>
#include <ostream>
#include <array>

template<class T>
std::ostream& PrintVector(std::ostream& s, const T& vec)
{
    bool first(true);
    for (auto &el : vec) {
        if (first) first = false; else s << ", ";
        s << el;
    }
    return s;
}

namespace std {

    template<class T>
    ostream& operator<<(ostream& s, const vector<T>& vec)
    {
        return ::PrintVector(s, vec);
    }
    
    template<class T>
    ostream& operator<<(ostream& s, const set<T>& vec)
    {
        return ::PrintVector(s, vec);
    }
    
    template<class T, size_t N>
    ostream& operator<<(ostream& s, const array<T, N>& array)
    {
        return ::PrintVector(s, array);
    }

}

#endif /* TEST_OPERATORS_H */

