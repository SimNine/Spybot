#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class Program;
class GUITexture;
class ProgramDisplayActionButton;

class ProgramDisplayContainer : public GUIContainer {
public:
	ProgramDisplayContainer(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims);
	virtual ~ProgramDisplayContainer();

	void draw();
	void tick(int ms);
	bool mouseDown();

	void setCurrProg(Program* p);
	Program* getCurrProg();
protected:
private:
	Program* currProg_;

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