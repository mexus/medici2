#ifndef GUI_DECK_SELECTOR_H
#define GUI_DECK_SELECTOR_H

#include <cards/deck-selector.h>
#include <QWidget>
#include <set>

#include "card_selector.h"

class GuiDeckSelector : public QWidget {
Q_OBJECT
public:
	std::unique_ptr<DeckAbstractSelector> GetSelector() const;
private:
	void AddCardSelector(int suit = -1, int rank = -1, bool inversed = false);
	std::set<GuiCardSelector*> selectors;

};

#endif /* GUI_DECK_SELECTOR_H */
