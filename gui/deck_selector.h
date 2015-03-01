#ifndef GUI_DECK_SELECTOR_H
#define GUI_DECK_SELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QCheckBox>

#include <set>

#include <cards/deck-selector.h>
#include "card_selector.h"

class GuiDeckSelector : public QWidget {
Q_OBJECT
public:
	struct Config {
		int selectorMode;
		std::size_t positionBegin, positionEnd;
		bool enabled;
		std::vector<GuiCardSelector::Config> cards;
	};

	GuiDeckSelector(const Config&, QWidget* parent = 0);
	GuiDeckSelector(QWidget* parent = 0);

	std::unique_ptr<DeckAbstractSelector> GetSelector() const;
	Config GetConfig() const;
private:
	void AddCardSelector(const GuiCardSelector::Config&);
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

	void CreateElements();
	virtual void SetSpinBoxes();
	void CreateLayout();

	static QSpinBox* CreateSpinBox(int min, int max);

signals:
	void DeleteClicked();
};

#endif /* GUI_DECK_SELECTOR_H */
