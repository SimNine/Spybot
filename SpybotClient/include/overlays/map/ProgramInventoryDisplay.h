#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class ProgramInventoryDisplay : public GUIContainer {
public:
	ProgramInventoryDisplay(GUIContainer* parent, ANCHOR anchor, Coord displacement, Coord dimensions);
	virtual ~ProgramInventoryDisplay();

	void resetBounds();
	void updateContents();

	int getPageScroll();
	void setPageScroll(int amt);
protected:
private:
	GUIContainer* progContainer_;

	const int iconCol_ = 20;
	const int nameCol_ = 80;
	const int numCol_ = 210;
	const int sizeCol_ = 270;
	const int speedCol_ = 330;

	int pageScroll_ = 0;
};