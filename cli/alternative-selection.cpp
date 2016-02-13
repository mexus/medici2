#include "alternative-selection.h"
#include <algorithm>
#include <map>
#include <set>
#include <easylogging++.h>
#include <cards/standard-36-deck.h>
// #include <mixer/factory.h>
#include "and-condition.h"
#include "move-condition.h"
#include "medici-condition.h"
#include "one-card-condition.h"
#include "apply-to-range-condition.h"
#include "storage.h"
#include "extravagant-conditions.h"

namespace std {
static ostream& operator<<(ostream& s, const Sequence& sequence) {
    auto& parts = sequence.GetParts();
    if (parts.empty()) {
        s << "<empty>";
        return s;
    }
    size_t counter = 0;
    for (auto part : parts) {
        s << "\n\tPart #" << ++counter << ": position " << part.position << ", elements ("
          << part.cards.size() << ")";
        for (auto& card : part.cards) {
            s << " [" << (int)card.rank << " " << (int)card.suit << "]";
        }
    }
    return s;
}
}

void AlternativeSelection::Run() {
    Storage storage(standard_36_deck::kDeck);
    AndCondition main_cond;
    // main_cond.AddChild<OneCardCondition>(
    //     0, Card{standard_36_deck::Clubs, standard_36_deck::King});
    // main_cond.AddChild<OneCardCondition>(
    //     1, Card{standard_36_deck::Diamonds, standard_36_deck::Seven});
    // main_cond.AddChild<OneCardCondition>(
    //     8, Card{standard_36_deck::Clubs, standard_36_deck::Ace});
    // main_cond.AddChild<AnySuitCardCondition>(14, standard_36_deck::Ace, true);

    main_cond.AddChild<OneCardCondition>(
        7, Card{standard_36_deck::Diamonds, standard_36_deck::Ace});
    main_cond.AddChild<OneCardCondition>(
        8, Card{standard_36_deck::Clubs, standard_36_deck::Ace});
    main_cond.AddChild<OneCardCondition>(
        9, Card{standard_36_deck::Hearts, standard_36_deck::Ace});
    main_cond.AddChild<OneCardCondition>(
        10, Card{standard_36_deck::Spades, standard_36_deck::Ace});
    // main_cond.AddChild<AnyRankCardCondition>(8, standard_36_deck::Hearts);
    // main_cond.AddChild<OneCardCondition>(6, Card{0, 0});
    // main_cond.AddChild<ApplyToRangeCondition>(
    //     7, 10, std::make_shared<AnySuitCardCondition>(0, standard_36_deck::Ace));

    // main_cond.AddChild<OneCardCondition>(0, Card{standard_36_deck::Spades,
    // standard_36_deck::Ace});
    // main_cond.AddChild<OneCardCondition>(1, Card{standard_36_deck::Spades,
    // standard_36_deck::King});
    // main_cond.AddChild<MediciCondition>(0, 2);
    // main_cond.AddChild<MediciCondition>(8);

    ExtravagantPatience extravagant_patience;
    // 1..9 -- 3:52.13
    main_cond.AddChild<MediciCondition>(0, 9, &extravagant_patience);

    auto sequences = main_cond.GetVariants({}, storage);
    LOG(INFO) << "Total " << sequences.size() << " sequences";
    for (auto& sequence : sequences) {
        LOG(INFO) << sequence;
    }
}
