#ifndef TEST_MODULES_OPERATORS_H
#define TEST_MODULES_OPERATORS_H
#include <ostream>

struct Card;
class CardSelector;
#include <calculator/thread.h>

namespace std {
    ostream& operator<<(ostream& s, const Card& );
    ostream& operator<<(ostream& s, const CardSelector& cardSelector);
    ostream& operator<<(ostream& s, const calculator::Thread::RunParameters&);
}

#endif /* TEST_MODULES_OPERATORS_H */

