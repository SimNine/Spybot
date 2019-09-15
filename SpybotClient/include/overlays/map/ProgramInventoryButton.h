#pragma once

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class ProgramInventoryButton : public GUIObject {
public:
	ProgramInventoryButton(ANCHOR, Coord, PROGRAM, GUIContainer*);
	virtual ~ProgramInventoryButton();

	void setTransparency(int);
	bool mouseDown();
	bool mouseUp();
	void draw();
	void tick(int);
	void resetBounds();
protected:
private:
	SDL_Texture* icon_;
	PROGRAM progType_;
};