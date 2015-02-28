#include "deck_selector.h"

void GuiDeckSelector::AddCardSelector(int suit, int rank, bool inversed) {
	auto selector = new GuiCardSelector(suit, rank, inversed);
	selectors.insert(selector);
	QObject::connect(selector, &GuiCardSelector::DeleteClicked, [this, selector](){
			selectors.erase(selector);
			});
}

