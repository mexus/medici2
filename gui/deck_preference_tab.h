#ifndef GUI_DECK_PREFERENCE_TAB_H
#define GUI_DECK_PREFERENCE_TAB_H

#include <QWidget>
#include <QCheckBox>
#include "card_selector.h"

class DeckPreference : public QWidget {
	Q_OBJECT
public:
	struct Config {
		bool checkIChing;
		bool findMaximumConvolutions;
		Card targetCard;
	};
	DeckPreference(QWidget* parent = nullptr);
	DeckPreference(const Config& config, QWidget* parent = nullptr);
private:
	QCheckBox* iChingCheck;

	GuiCardSelector* targetCard;
	QCheckBox* findMaximumConvolutions;
	
	void CreateLayout();
};

#endif /* GUI_DECK_PREFERENCE_TAB_H */
