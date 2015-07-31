#define private public
#include <cards/deck-selector.h>
#undef private

#include "operators.h"

namespace std {

ostream& operator<<(ostream& s, const Card& card) {
    return s << "suit #" << (int)card.suit << " rank #" << (int)card.rank;
}

ostream& operator<<(ostream& s, const CardSelector& cardSelector) {
    s << "{";
    if (cardSelector.config.straight)
        s << "straight";
    else
        s << "inverse";
    s << ", ";
    bool printSeparator = false;
    if (cardSelector.config.suitSet) {
        s << "suit #" << static_cast<int>(cardSelector.config.suit);
        printSeparator = true;
    }
    if (cardSelector.config.rankSet) {
        if (printSeparator)
            s << ", ";
        s << "rank #" << static_cast<int>(cardSelector.config.rank);
    }
    s << "}";
    return s;
}

ostream& operator<<(ostream& s, const calculator::ExecutionParameters& params) {
    using namespace chrono;
    return s << "total " << params.checkedDecks << " decks, suitable "
             << params.suitableDecks << " decks, "
             << "running for "
             << duration_cast<duration<float>>(params.runningTime).count() << "s";
}
}
