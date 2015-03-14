#ifndef GUI_CARDS_TRANSLATIONS_H
#define GUI_CARDS_TRANSLATIONS_H

#include <QObject>
#include <map>

#include <cards/standard-36-deck.h>

class CardsTranslations : QObject {
    Q_OBJECT
public:
    CardsTranslations();

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

    static const QString& CardName(const std::map<Card, QString>& translations, const Card&);
};

#endif /* GUI_CARDS_TRANSLATIONS_H */
