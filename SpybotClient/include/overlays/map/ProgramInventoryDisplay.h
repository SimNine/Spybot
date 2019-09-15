#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class ProgramInventoryDisplay : public GUIContainer {
public:
	ProgramInventoryDisplay(ANCHOR, Coord, Coord, GUIContainer*);
	virtual ~ProgramInventoryDisplay();

	void resetBounds();
	void updateContents();
protected:
private:
};