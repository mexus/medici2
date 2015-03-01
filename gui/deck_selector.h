#ifndef GUI_DECK_SELECTOR_H
#define GUI_DECK_SELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>

#include <set>

#include <cards/deck-selector.h>
#include "card_selector.h"

class GuiDeckSelector : public QWidget {
Q_OBJECT
public:
	GuiDeckSelector(QWidget* parent = 0);

	std::unique_ptr<DeckAbstractSelector> GetSelector() const;
private:
	void AddCardSelector(int suit = -1, int rank = -1, bool inversed = false);
	std::set<GuiCardSelector*> selectors;
	
	enum SelectorMode {
		SELECT_ALL,
		SELECT_ONE
	};

	QComboBox* selectorMode;
	QSpinBox *positionStart, *positionEnd;

	QHBoxLayout* selectorsLayout;

	void CreateElements();
	virtual void SetSpinBoxes();
	void CreateLayout();

	static QSpinBox* CreateSpinBox(int min, int max);
};

#endif /* GUI_DECK_SELECTOR_H */
