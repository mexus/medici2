#ifndef GUI_SELECTORS_GUI_CARD_SELECTOR_H
#define GUI_SELECTORS_GUI_CARD_SELECTOR_H

#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QFrame>
#include <QJsonObject>
#include <QVBoxLayout>
#include <cards/card-selector.h>
#include <helpers/cards_translations.h>

class GuiCardSelector : public QFrame {
Q_OBJECT
public:
    GuiCardSelector(const CardsTranslations&, const QJsonObject&, bool multipleAllowed = true);
    GuiCardSelector(const CardsTranslations&, bool multipleAllowed = true);
    CardSelector GetSelector(bool &ok);

    QJsonObject GetConfig() const;
    Card GetCard() const;
    void Highlight();
    void AddWidget(QWidget*);
private:
    const CardsTranslations& cardsTranslations;
    QVBoxLayout *innerLayout;
    QComboBox *suit, *rank;
    QCheckBox *inverse;
    bool multipleAllowed;

    void CreateObjects();
    void CreateLayout();
    virtual void PopulateSuits();
    virtual void PopulateRanks();
};

#endif /* GUI_SELECTORS_GUI_CARD_SELECTOR_H */
