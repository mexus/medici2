#include <ostream>

#include <cards/sequence.h>

namespace cards {

void PrintTo(const SimpleSequence& sequence, ::std::ostream* s);
void PrintTo(const Sequence& sequence, ::std::ostream* s);
}
