#ifndef TEST_OPERATORS_H
#define TEST_OPERATORS_H
#include <ostream>

#define private public
#include <cards/deck.h>
#include <cards/deck-selector.h>
#undef private
#include <calculator/thread.h>

namespace std {
	ostream& operator<<(ostream& s, const Card& );
	ostream& operator<<(ostream& s, const CardSelector& cardSelector);
	ostream& operator<<(ostream& s, const calculator::Thread::RunParameters&);
}

#endif /* TEST_OPERATORS_H */

