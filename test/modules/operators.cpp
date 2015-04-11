#define private public
#include <cards/deck.h>
#include <cards/deck-selector.h>
#undef private

#include "operators.h"

namespace std {

    ostream& operator<<(ostream& s, const Card& card)
    {
        return s << "suit #" << (int)card.suit.value << " rank #" << (int)card.rank.value;
    }

    ostream& operator<<(ostream& s, const CardSelector& cardSelector)
    {
        s << "{";
        if (cardSelector.straight)
            s << "straight";
        else
            s << "inverse";
        s << ", ";
        bool printSeparator = false;
        if (cardSelector.suitSet) {
            s << "suit #" << static_cast<int>(cardSelector.suit.value);
            printSeparator = true;
        }
        if (cardSelector.rankSet) {
            if (printSeparator)
                s << ", ";
            s << "rank #" << static_cast<int>(cardSelector.rank.value);
        }
        s << "}";
        return s;
    }

    ostream& operator<<(ostream& s, const calculator::Thread::RunParameters& params)
    {
        using namespace chrono;
        return s << "total " << params.checkedDecks << " decks, suitable " << params.suitableDecks << " decks, " <<
            "running for " << duration_cast<duration<float>>(params.runningTime).count() << "s";

    }

}

