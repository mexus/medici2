#ifndef GUI_CARD_SELECTOR_H
#define GUI_CARD_SELECTOR_H

#include <cards/card-selector.h>
#include <QWidget>
#include <QComboBox>
#include <QCheckBox>

#include <stdexcept>

class GuiCardSelector : public QWidget {
Q_OBJECT
public:
	struct Config {
		int suit;
		int rank;
		bool inversed;
	};

	class NothingSelected : public std::exception {
	};

	GuiCardSelector(const Config&, QWidget* parent = nullptr);
	GuiCardSelector(QWidget* parent = nullptr);
	CardSelector GetSelector() const;

	Config GetConfig() const;
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
