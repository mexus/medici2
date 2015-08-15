#include "medici.h"
#include <operators.h>
#include <comparisons.h>
#include "operators.h"

TestMedici::TestMedici() : TestFW("medici") {
}

bool TestMedici::Tests() {
    using namespace standard_36_deck;
    std::vector<Card> convergingDeck1{{{Jack, Spades},
                                       {Nine, Diamonds},
                                       {Ten, Spades},
                                       {Queen, Diamonds},
                                       {Seven, Hearts},
                                       {Seven, Diamonds},
                                       {Queen, Spades},
                                       {Jack, Hearts},
                                       {Ten, Diamonds},
                                       {Jack, Clubs},
                                       {Ten, Hearts},
                                       {Seven, Spades},
                                       {Nine, Spades},
                                       {Eight, Spades},
                                       {King, Diamonds},
                                       {Nine, Hearts},
                                       {Six, Hearts},
                                       {Ace, Spades},
                                       {Ace, Diamonds},
                                       {Seven, Clubs},
                                       {Jack, Diamonds},
                                       {Ace, Clubs},
                                       {Eight, Hearts},
                                       {Ace, Hearts},
                                       {Ten, Clubs},
                                       {Six, Diamonds},
                                       {Eight, Clubs},
                                       {King, Spades},
                                       {King, Hearts},
                                       {Nine, Clubs},
                                       {Queen, Hearts},
                                       {Six, Spades},
                                       {King, Clubs},
                                       {Six, Clubs},
                                       {Eight, Diamonds},
                                       {Queen, Clubs}}};
    PatienceInfo deck1Info;
    deck1Info.stationars = {{Jack, Spades},
                            {Queen, Diamonds},
                            {Seven, Diamonds},
                            {Jack, Hearts},
                            {Ten, Hearts},
                            {Seven, Spades},
                            {King, Diamonds},
                            {Six, Hearts},
                            {Ace, Spades},
                            {Ace, Diamonds},
                            {Ace, Clubs},
                            {Ace, Hearts},
                            {Ten, Clubs},
                            {Eight, Clubs},
                            {King, Hearts},
                            {Six, Spades},
                            {Six, Clubs}};
    deck1Info.convolutions = {{{Ten, Hearts}, 1},
                              {{Queen, Hearts}, 2},
                              {{Ace, Hearts}, 10},
                              {{Seven, Diamonds}, 1},
                              {{Jack, Diamonds}, 1},
                              {{Queen, Diamonds}, 1},
                              {{Six, Clubs}, 6},
                              {{Eight, Clubs}, 1},
                              {{Jack, Clubs}, 3},
                              {{Queen, Clubs}, 4},
                              {{Ace, Clubs}, 2},
                              {{Eight, Spades}, 1},
                              {{Ten, Spades}, 1}};

    std::vector<Card> notConvergingDeck2{{{Ten, Hearts},
                                          {Eight, Clubs},
                                          {Ace, Hearts},
                                          {Jack, Clubs},
                                          {Seven, Spades},
                                          {Ten, Clubs},
                                          {Nine, Spades},
                                          {Seven, Hearts},
                                          {Nine, Hearts},
                                          {Ace, Diamonds},
                                          {Six, Hearts},
                                          {Queen, Spades},
                                          {Eight, Hearts},
                                          {King, Diamonds},
                                          {Ace, Spades},
                                          {Ten, Spades},
                                          {Jack, Diamonds},
                                          {Eight, Spades},
                                          {Seven, Clubs},
                                          {Nine, Diamonds},
                                          {Six, Spades},
                                          {Queen, Hearts},
                                          {King, Hearts},
                                          {Jack, Spades},
                                          {Jack, Hearts},
                                          {King, Spades},
                                          {Nine, Clubs},
                                          {Queen, Diamonds},
                                          {Ten, Diamonds},
                                          {Eight, Diamonds},
                                          {Six, Clubs},
                                          {Queen, Clubs},
                                          {Seven, Diamonds},
                                          {Ace, Clubs},
                                          {Six, Diamonds},
                                          {King, Clubs}}};

    std::vector<Card> convergingDeck3{{{Jack, Diamonds},
                                       {Nine, Clubs},
                                       {Ten, Hearts},
                                       {Seven, Clubs},
                                       {Seven, Diamonds},
                                       {Queen, Diamonds},
                                       {Queen, Spades},
                                       {Seven, Spades},
                                       {Jack, Hearts},
                                       {King, Spades},
                                       {Six, Spades},
                                       {Eight, Spades},
                                       {Ace, Spades},
                                       {Eight, Clubs},
                                       {Ace, Clubs},
                                       {Seven, Hearts},
                                       {Nine, Hearts},
                                       {Ten, Clubs},
                                       {Ace, Diamonds},
                                       {Eight, Hearts},
                                       {Ace, Hearts},
                                       {King, Clubs},
                                       {Queen, Clubs},
                                       {Queen, Hearts},
                                       {Ten, Spades},
                                       {Nine, Spades},
                                       {King, Hearts},
                                       {Jack, Spades},
                                       {King, Diamonds},
                                       {Six, Hearts},
                                       {Nine, Diamonds},
                                       {Eight, Diamonds},
                                       {Jack, Clubs},
                                       {Ten, Diamonds},
                                       {Six, Clubs},
                                       {Six, Diamonds}}};
    PatienceInfo deck3Info;
    deck3Info.stationars = {{Jack, Diamonds},
                            {Nine, Clubs},
                            {Seven, Clubs},
                            {Seven, Diamonds},
                            {Queen, Spades},
                            {Seven, Spades},
                            {King, Spades},
                            {Ace, Clubs},
                            {Nine, Hearts},
                            {Ace, Diamonds},
                            {Ace, Hearts},
                            {King, Clubs},
                            {Queen, Hearts},
                            {Nine, Spades},
                            {King, Diamonds},
                            {Nine, Diamonds},
                            {Eight, Diamonds}};
    deck3Info.convolutions = {{{Ace, Hearts}, 10},
                              {{Six, Diamonds}, 1},
                              {{Nine, Diamonds}, 1},
                              {{Ten, Diamonds}, 4},
                              {{King, Diamonds}, 1},
                              {{Six, Clubs}, 2},
                              {{Seven, Clubs}, 1},
                              {{Eight, Clubs}, 1},
                              {{Ace, Clubs}, 1},
                              {{Eight, Spades}, 1},
                              {{Jack, Spades}, 8},
                              {{King, Spades}, 2},
                              {{Ace, Spades}, 1}};

    return Test(convergingDeck1, true, deck1Info) && Test(notConvergingDeck2, false) &&
           Test(convergingDeck3, true, deck3Info);
}

bool TestMedici::Test(const std::vector<Card>& deck, bool etalonConverges,
                      const PatienceInfo& etalonInfo) {
    PatienceInfo resultInfo;
    bool resultConverges = TryToConverge(deck, resultInfo);

    bool res;
    if (resultConverges != etalonConverges) {
        std::ostringstream s;
        s << "Deck should ";
        if (!etalonConverges)
            s << "not ";
        s << "converge, but it does ";
        if (!resultConverges)
            s << "not";
        res = false;
        LOG(ERROR) << s.str();
    } else if (resultConverges)
        res = Compare(resultInfo, etalonInfo);
    else
        res = true;
    if (!res) {
        LOG(ERROR) << "Failed on {" << deck << "}";
    }
    return res;
}

bool TestMedici::Compare(const PatienceInfo& etalon, const PatienceInfo& result) {
    if (!::Compare(etalon.convolutions, result.convolutions)) {
        LOG(ERROR) << "Convolutions check failed";
        return false;
    } else if (!::Compare(etalon.stationars, result.stationars)) {
        LOG(ERROR) << "Stationars check failed";
        return false;
    } else
        return true;
}
