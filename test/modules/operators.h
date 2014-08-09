#ifndef TEST_OPERATORS_H
#define TEST_OPERATORS_H
#include <ostream>
#include <cards/deck.h>
#include <cards/deck-selector.h>

std::ostream& operator<<(std::ostream& s, const Card& );
std::ostream& operator<<(std::ostream& s, const CardSelector& cardSelector);

#endif /* TEST_OPERATORS_H */

