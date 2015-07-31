#ifndef GUI_HELPERS_CARDS_TRANSLATIONS_H
#define GUI_HELPERS_CARDS_TRANSLATIONS_H

#include <QObject>
#include <map>

#include <cards/standard-36-deck.h>

class CardsTranslations : QObject {
    Q_OBJECT
public:
    CardsTranslations();

    const QString& SuitLongName(std::uint_fast8_t) const;
    const QString& RankLongName(std::uint_fast8_t) const;

    const QString& CardShortName(const Card&) const;
    const QString& CardLongName(const Card&) const;
    void PopulateTranslations();

private:
    std::map<std::uint_fast8_t, QString> suitsTranslationsShort, suitsTranslationsLong;
    std::map<std::uint_fast8_t, QString> ranksTranslationsShort, ranksTranslationsLong;
    std::map<Card, QString> cardsTranslationsShort, cardsTranslationsLong;

    void PopulateRanks();
    void PopulateSuits();
    void AddCard(const Card&);

    template <class T>
    static const QString& Name(const std::map<T, QString>& translations, const T&);
};

#endif /* GUI_HELPERS_CARDS_TRANSLATIONS_H */
