#include "cards_translations.h"
#include <stdexcept>

CardsTranslations::CardsTranslations() : QObject(nullptr)
{
    PopulateTranslations();
}

void CardsTranslations::PopulateRanks()
{
    ranksTranslationsLong[standard_36_deck::Six]   = tr("Six");
    ranksTranslationsLong[standard_36_deck::Seven] = tr("Seven");
    ranksTranslationsLong[standard_36_deck::Eight] = tr("Eight");
    ranksTranslationsLong[standard_36_deck::Nine]  = tr("Nine");
    ranksTranslationsLong[standard_36_deck::Ten]   = tr("Ten");
    ranksTranslationsLong[standard_36_deck::Jack]  = tr("Jack");
    ranksTranslationsLong[standard_36_deck::Queen] = tr("Queen");
    ranksTranslationsLong[standard_36_deck::King]  = tr("King");
    ranksTranslationsLong[standard_36_deck::Ace]   = tr("Ace");

    ranksTranslationsShort[standard_36_deck::Six]   = "6";
    ranksTranslationsShort[standard_36_deck::Seven] = "7";
    ranksTranslationsShort[standard_36_deck::Eight] = "8";
    ranksTranslationsShort[standard_36_deck::Nine]  = "9";
    ranksTranslationsShort[standard_36_deck::Ten]   = "X";
    ranksTranslationsShort[standard_36_deck::Jack]  = tr("J");
    ranksTranslationsShort[standard_36_deck::Queen] = tr("Q");
    ranksTranslationsShort[standard_36_deck::King]  = tr("K");
    ranksTranslationsShort[standard_36_deck::Ace]   = tr("A");
}

void CardsTranslations::PopulateSuits()
{
    suitsTranslationsLong[standard_36_deck::Diamonds] = tr("Diamonds");
    suitsTranslationsLong[standard_36_deck::Spades]   = tr("Spades");
    suitsTranslationsLong[standard_36_deck::Hearts]   = tr("Hearts");
    suitsTranslationsLong[standard_36_deck::Clubs]    = tr("Clubs");

    suitsTranslationsShort[standard_36_deck::Diamonds] = tr("d");
    suitsTranslationsShort[standard_36_deck::Spades]   = tr("s");
    suitsTranslationsShort[standard_36_deck::Hearts]   = tr("h");
    suitsTranslationsShort[standard_36_deck::Clubs]    = tr("c");
}

void CardsTranslations::AddCard(const Card& card)
{
    auto &suit = card.suit;
    auto &rank = card.rank;
    QString rankLong, rankShort, suitLong, suitShort;

    auto itRankLong = ranksTranslationsLong.find(rank);
    if (itRankLong == ranksTranslationsLong.end())
        throw std::logic_error("Rank not found");
    else
        rankLong = itRankLong->second;

    auto itRankShort = ranksTranslationsShort.find(rank);
    if (itRankShort == ranksTranslationsShort.end())
        throw std::logic_error("Rank not found");
    else
        rankShort = itRankShort->second;

    auto itSuitLong = suitsTranslationsLong.find(suit);
    if (itSuitLong == suitsTranslationsLong.end())
        throw std::logic_error("Suit not found");
    else
        suitLong = itSuitLong->second;

    auto itSuitShort = suitsTranslationsShort.find(suit);
    if (itSuitShort == suitsTranslationsShort.end())
        throw std::logic_error("Suit not found");
    else
        suitShort = itSuitShort->second;

    cardsTranslationsLong[{suit, rank}]  = rankLong + " " + suitLong;
    cardsTranslationsShort[{suit, rank}] = rankShort      + suitShort;
}

void CardsTranslations::PopulateTranslations()
{
    PopulateRanks();
    PopulateSuits();
    for (auto &card : standard_36_deck::kDeck)
        AddCard(card);
}

template<class T>
const QString& CardsTranslations::Name(const std::map<T, QString>& translations, const T& card)
{
    auto it = translations.find(card);
    if (it == translations.end())
        throw std::logic_error("Unknown card");
    else
        return it->second;
}

const QString& CardsTranslations::CardShortName(const Card& card) const {
    return Name(cardsTranslationsShort, card);
}

const QString& CardsTranslations::CardLongName(const Card& card) const {
    return Name(cardsTranslationsLong, card);
}

const QString& CardsTranslations::SuitLongName(std::uint_fast8_t suit) const {
    return Name(suitsTranslationsLong, suit);
}

const QString& CardsTranslations::RankLongName(std::uint_fast8_t rank) const {
    return Name(ranksTranslationsLong, rank);
}

