#ifndef GUI_WIDGETS_SELECTORS_GUI_DECK_SELECTOR_H
#define GUI_WIDGETS_SELECTORS_GUI_DECK_SELECTOR_H

#include <QFrame>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <set>
#include <cards/deck-selector.h>
#include "gui_card_selector.h"

class GuiDeckSelector : public QFrame {
Q_OBJECT
public:
    GuiDeckSelector(const CardsTranslations&, const QJsonObject&);
    GuiDeckSelector(const CardsTranslations&, GuiCardSelector*);
    GuiDeckSelector(const CardsTranslations&, bool newCard = true);

    std::unique_ptr<DeckAbstractSelector> GetSelector() const;
    QJsonObject GetConfig() const;
    void AddButton(QPushButton*);
private:
    const CardsTranslations& cardsTranslations;
    void AddCardSelector(const QJsonObject&);
    void AddCardSelector();
    void AddCardSelector(GuiCardSelector*, bool removeButton = true);
    std::set<GuiCardSelector*> selectors;
    
    enum SelectorMode {
        SELECT_ALL,
        SELECT_ONE
    };

    QComboBox *selectorMode;
    QSpinBox *positionStart, *positionEnd;
    QCheckBox *enabled;

    QVBoxLayout *configLayout;
    QHBoxLayout *selectorsLayout;

    void CreateObjects();
    virtual void SetSpinBoxes();
    void CreateLayout();

    static QSpinBox* CreateSpinBox(int min, int max);
};

#endif /* GUI_WIDGETS_SELECTORS_GUI_DECK_SELECTOR_H */
