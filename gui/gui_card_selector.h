#ifndef GUI_GUI_CARD_SELECTOR_H
#define GUI_GUI_CARD_SELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QFrame>
#include <QJsonObject>
#include <cards/card-selector.h>
#include "gui_exception.h"

class GuiCardSelector : public QWidget {
Q_OBJECT
public:
    class NoSuitNoRank : public GuiException {
    public:
        NoSuitNoRank(GuiCardSelector*);
    };

    GuiCardSelector(const QJsonObject&, bool anyAllowed = true, bool inverseAllowed = true, QWidget* parent = nullptr);
    GuiCardSelector(bool anyAllowed = true, bool inverseAllowed = true, QWidget* parent = nullptr);
    CardSelector GetSelector();

    QJsonObject GetConfig() const;
    void Highlight();
    void HideRemoveButton();
private:
    QComboBox *suit, *rank;
    QCheckBox *inverse;
    QFrame *frame;
    QPushButton *removeButton;
    bool anyAllowed, inverseAllowed;

    void CreateObjects();
    void CreateLayout();
    virtual void PopulateSuits();
    virtual void PopulateRanks();

signals:
    void DeleteClicked();
};

#endif /* GUI_GUI_CARD_SELECTOR_H */
