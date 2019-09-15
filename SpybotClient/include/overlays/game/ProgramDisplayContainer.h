#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class ProgramMirror;
class GUITexture;
class ProgramDisplayActionButton;

class ProgramDisplayContainer : public GUIContainer {
public:
	ProgramDisplayContainer(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims);
	virtual ~ProgramDisplayContainer();

	void draw();
	void tick(int ms);
	bool mouseDown();

	void setCurrProg(ProgramMirror* p);
	ProgramMirror* getCurrProg();
protected:
private:
	ProgramMirror* currProg_;

	GUITexture* iconBacking_;
	GUITexture* icon_;
	GUITexture* nameText_;
	GUITexture* ownerText_;
	GUITexture* healthText_;
	GUITexture* movesText_;
	GUITexture* actionsText_;
	GUITexture* descText_;

	GUIContainer* programInfoContainer_;
	GUIContainer* actionButtonContainer_;
};