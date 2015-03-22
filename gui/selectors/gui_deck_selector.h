#ifndef GUI_SELECTORS_GUI_DECK_SELECTOR_H
#define GUI_SELECTORS_GUI_DECK_SELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <set>
#include <cards/deck-selector.h>
#include "gui_card_selector.h"
#include <helpers/gui_exception.h>

class GuiDeckSelector : public QWidget {
Q_OBJECT
public:
    class NoCards : public GuiException {
    };

    GuiDeckSelector(const QJsonObject&, QWidget* parent = 0);
    GuiDeckSelector(bool newCard = true, QWidget* parent = 0);

    std::unique_ptr<DeckAbstractSelector> GetSelector() const;
    QJsonObject GetConfig() const;
private:
    void AddCardSelector(const QJsonObject&);
    void AddCardSelector();
    void AddCardSelector(GuiCardSelector*);
    std::set<GuiCardSelector*> selectors;
    
    enum SelectorMode {
        SELECT_ALL,
        SELECT_ONE
    };

    QComboBox* selectorMode;
    QSpinBox *positionStart, *positionEnd;
    QCheckBox *enabled;

    QHBoxLayout* selectorsLayout;

    void CreateObjects();
    virtual void SetSpinBoxes();
    void CreateLayout();

    static QSpinBox* CreateSpinBox(int min, int max);

signals:
    void DeleteClicked();
};

#endif /* GUI_SELECTORS_GUI_DECK_SELECTOR_H */
