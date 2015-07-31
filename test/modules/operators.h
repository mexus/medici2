#ifndef TEST_MODULES_OPERATORS_H
#define TEST_MODULES_OPERATORS_H
#include <ostream>

struct Card;
class CardSelector;
#include <calculator/thread.h>

namespace std {
ostream& operator<<(ostream&, const Card&);
ostream& operator<<(ostream&, const CardSelector&);

ostream& operator<<(ostream&, const typename calculator::ExecutionParameters&);
}

#endif /* TEST_MODULES_OPERATORS_H */
