#ifndef GUI_CARD_SELECTOR_H
#define GUI_CARD_SELECTOR_H

#include <cards/card-selector.h>
#include <QWidget>
#include <QComboBox>
#include <QCheckBox>

class GuiCardSelector : public QWidget {
Q_OBJECT
public:
	GuiCardSelector(int suit, int rank, bool inversed, QWidget* parent = nullptr);
	CardSelector GetSelector() const;
private:
	QComboBox *suit, *rank;
	QCheckBox *inverse;

	void CreateElements();
	void CreateLayout();
	virtual void PopulateSuits();
	virtual void PopulateRanks();

signals:
	void DeleteClicked();
};

#endif /* GUI_CARD_SELECTOR_H */
