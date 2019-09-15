#pragma once

#include "Standard.h"
#include "GUIObject.h"
#include "ProgramAction.h"

class GUIContainer;

class ProgramDisplayActionButton : public GUIObject {
public:
	ProgramDisplayActionButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims, ProgramAction action, int actionNum);
	virtual ~ProgramDisplayActionButton();

	void setTransparency(int);
	bool mouseDown();
	bool mouseUp();
	void draw();
	void tick(int);
	void resetBounds();
protected:
private:
	ProgramAction action_;
	int actionNum_;
};