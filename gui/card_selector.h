#ifndef GUI_CARD_SELECTOR_H
#define GUI_CARD_SELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QFrame>

#include <cards/card-selector.h>
#include "exception.h"

class GuiCardSelector : public QWidget {
Q_OBJECT
public:
	class NoSuitNoRank : public GuiException {
	public:
		NoSuitNoRank(GuiCardSelector*);
	};

	struct Config {
		int suit;
		int rank;
		bool inversed;
	};

	GuiCardSelector(const Config&, QWidget* parent = nullptr);
	GuiCardSelector(QWidget* parent = nullptr);
	CardSelector GetSelector();

	Config GetConfig() const;
	void Highlight();
private:
	QComboBox *suit, *rank;
	QCheckBox *inverse;
	QFrame *frame;

	void CreateElements();
	void CreateLayout();
	virtual void PopulateSuits();
	virtual void PopulateRanks();

signals:
	void DeleteClicked();
};

#endif /* GUI_CARD_SELECTOR_H */
