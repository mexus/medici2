#ifndef GUI_WIDGETS_DECK_PREFERENCE_H
#define GUI_WIDGETS_DECK_PREFERENCE_H

#include <QWidget>
#include <QCheckBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QVBoxLayout>
#include <set>
#include <medici/patience-selector.h>
#include <widgets/selectors/gui_deck_selector.h>

class DeckPreference : public QWidget {
    Q_OBJECT
public:
    DeckPreference(const CardsTranslations&);
    DeckPreference(const CardsTranslations&, const QJsonObject&);

    QJsonObject GetConfig() const;
    DeckSelectors GetDeckSelectors() const;
    medici::PPatienceSelector GetPatienceSelector() const;
private:
    const CardsTranslations& cardsTranslations;
    QCheckBox* iChingCheck;
    GuiCardSelector* targetCard;
    QCheckBox* findMaximumConvolutions;
    QVBoxLayout* decksLayout;
    std::set<GuiDeckSelector*> deckSelectors;
    
    void CreateLayout();
    medici::PPatienceSelector GetSelector() const;

    void AddDeckSelector(GuiDeckSelector*);
    void AddNewDeckSelector();
    void RemoveDeckSelector(GuiDeckSelector*);
};

#endif /* GUI_WIDGETS_DECK_PREFERENCE_H */
