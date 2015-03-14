#ifndef GUI_DECK_PREFERENCE_H
#define GUI_DECK_PREFERENCE_H

#include <QWidget>
#include <QCheckBox>
#include <QJsonObject>
#include "gui_card_selector.h"

class DeckPreference : public QWidget {
    Q_OBJECT
public:
    DeckPreference(QWidget* parent = nullptr);
    DeckPreference(const QJsonObject&, QWidget* parent = nullptr);

    QJsonObject GetConfig() const;
private:
    QCheckBox* iChingCheck;

    GuiCardSelector* targetCard;
    QCheckBox* findMaximumConvolutions;
    
    void CreateLayout();
};

#endif /* GUI_DECK_PREFERENCE_H */
