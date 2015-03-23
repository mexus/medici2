#ifndef GUI_HELPERS_CARDS_TRANSLATIONS_H
#define GUI_HELPERS_CARDS_TRANSLATIONS_H

#include <QObject>
#include <map>

#include <cards/standard-36-deck.h>

class CardsTranslations : QObject {
    Q_OBJECT
public:
    CardsTranslations();

    const QString& SuitLongName(const Card::Suit&) const;
    const QString& RankLongName(const Card::Rank&) const;

    const QString& CardShortName(const Card&) const;
    const QString& CardLongName(const Card&) const;
    void PopulateTranslations();

private:
    std::map<Card::Suit, QString> suitsTranslationsShort, suitsTranslationsLong;
    std::map<Card::Rank, QString> ranksTranslationsShort, ranksTranslationsLong;
    std::map<Card, QString> cardsTranslationsShort, cardsTranslationsLong;

    void PopulateRanks();
    void PopulateSuits();
    void AddCard(const Card&);

    template<class T>
    static const QString& Name(const std::map<T, QString>& translations, const T&);
};

#endif /* GUI_HELPERS_CARDS_TRANSLATIONS_H */
