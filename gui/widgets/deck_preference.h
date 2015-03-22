#ifndef GUI_WIDGETS_DECK_PREFERENCE_H
#define GUI_WIDGETS_DECK_PREFERENCE_H

#include <QWidget>
#include <QCheckBox>
#include <QJsonObject>
#include <medici/patience-selector.h>
#include <selectors/gui_card_selector.h>

class DeckPreference : public QWidget {
    Q_OBJECT
public:
    DeckPreference(QWidget* parent = nullptr);
    DeckPreference(const QJsonObject&, QWidget* parent = nullptr);

    QJsonObject GetConfig() const;
    medici::PPatienceSelector GetSelector() const;
private:
    QCheckBox* iChingCheck;

    GuiCardSelector* targetCard;
    QCheckBox* findMaximumConvolutions;
    
    void CreateLayout();
};

#endif /* GUI_WIDGETS_DECK_PREFERENCE_H */
