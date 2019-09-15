#include "Standard.h"
#include "ProgramDisplayContainer.h"

#include "Global.h"
#include "ProgramMirror.h"
#include "ResourceLoader.h"
#include "GameOverlay.h"
#include "Data.h"
#include "MiscUtil.h"
#include "ProgramActionMirror.h"
#include "Client.h"
#include "PlayerMirror.h"
#include "GameMirror.h"
#include "GUITexture.h"
#include "Client.h"
#include "ClientMirror.h"
#include "ProgramDisplayActionButton.h"
#include "ConnectionManager.h"
#include "GUIEffectFade.h"

ProgramDisplayContainer::ProgramDisplayContainer(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims)
	: GUIContainer(parent, a, disp, dims, _color_clear) {
	currProg_ = NULL;

	programInfoContainer_ = new GUIContainer(this, ANCHOR_NORTHEAST, { 0, 0 }, { this->bounds_.w, 200 }, _color_bkg_standard);
	this->addObject(programInfoContainer_);

	iconBacking_ = new GUITexture(programInfoContainer_, ANCHOR_NORTHEAST, { -5, 5 }, { 150, 150 }, _program_core_100px);
	programInfoContainer_->addObject(iconBacking_);

	icon_ = NULL;
	nameText_ = NULL;
	ownerText_ = NULL;
	healthText_ = NULL;
	movesText_ = NULL;
	actionsText_ = NULL;

	actionButtonContainer_ = new GUIContainer(this, ANCHOR_NORTHEAST, { 0, 210 }, { this->bounds_.w, 400 }, _color_bkg_standard);
	this->addObject(actionButtonContainer_);
}

ProgramDisplayContainer::~ProgramDisplayContainer() {
	//dtor
}

void ProgramDisplayContainer::tick(int ms) {
	GUIContainer::tick(ms);

	// check for a current program
	if (currProg_ == NULL)
		return;

	// refresh the owner text
	if (ownerText_ != NULL) {
		programInfoContainer_->removeObject(ownerText_);
		delete ownerText_;
	}

	ClientMirror* cMirr = _connectionManager->getClientMirrorByPlayerID(currProg_->getOwner()->getPlayerID());
	std::string ownerText = (cMirr == NULL) ? "AI" : cMirr->name_;
	ownerText_ = new GUITexture(programInfoContainer_, ANCHOR_NORTHWEST, { 5, 5 }, "Owner: " + ownerText, 30);
	programInfoContainer_->addObject(ownerText_);

	// refresh the health text
	if (healthText_ != NULL) {
		programInfoContainer_->removeObject(healthText_);
		delete healthText_;
	}
	std::string healthText = to_string(currProg_->getHealth()) + "/" + to_string(currProg_->getMaxHealth());
	healthText_ = new GUITexture(programInfoContainer_, ANCHOR_NORTHWEST, { 5, 40 }, "Health: " + healthText, 30);
	programInfoContainer_->addObject(healthText_);

	// refresh the moves text
	if (movesText_ != NULL) {
		programInfoContainer_->removeObject(movesText_);
		delete movesText_;
	}
	std::string movesText = to_string(currProg_->getMoves()) + "/" + to_string(currProg_->getMaxMoves());
	movesText_ = new GUITexture(programInfoContainer_, ANCHOR_NORTHWEST, { 5, 75 }, "Moves: " + movesText, 30);
	programInfoContainer_->addObject(movesText_);

	// refresh the actions text
	if (actionsText_ != NULL) {
		programInfoContainer_->removeObject(actionsText_);
		delete actionsText_;
	}
	std::string actionsText = to_string(currProg_->getActionsLeft()) + "/" + to_string(1);
	actionsText_ = new GUITexture(programInfoContainer_, ANCHOR_NORTHWEST, { 5, 110 }, "Actions: " + actionsText, 30);
	programInfoContainer_->addObject(actionsText_);
}

void ProgramDisplayContainer::draw() {
	// draw bounds
	if (_debug >= DEBUG_NORMAL)
		drawBounds();

	// draw all the permanent stuff
	GUIContainer::drawBkg();
	GUIContainer::drawContents();
}

bool ProgramDisplayContainer::mouseDown() {
	GUIContainer::mouseDown();
	return true;
}

void ProgramDisplayContainer::setCurrProg(ProgramMirror* p) {
	// fade in / out
	if (p == NULL) {
		this->addEffect(new GUIEffectFade(0, 500, 255, 0));
	} else if (p != NULL && currProg_ == NULL) {
		this->addEffect(new GUIEffectFade(0, 500, 0, 255));
	}

	currProg_ = p;
	if (p == NULL)
		return;

	// replace icon
	if (icon_ != NULL) {
		this->removeObject(icon_);
		delete icon_;
	}
	icon_ = new GUITexture(this, ANCHOR_NORTHEAST, { -20, 5 }, { 135, 135 }, _program_icons[p->getType()], false);
	this->addObject(icon_);

	// replace name
	if (nameText_ != NULL) {
		this->removeObject(nameText_);
		delete nameText_;
	}
	nameText_ = new GUITexture(this, ANCHOR_NORTHEAST, { -5, 155 }, p->getName(), 38);
	this->addObject(nameText_);

	// remove actionbuttons
	while (actionButtonContainer_->getContents()->getLength() > 0) {
		GUIObject* curr = actionButtonContainer_->getContents()->poll();
		this->removeObject(curr);
		delete curr;
	}

	// replace actionbuttons
	int yDisp = 10;
	int index = 0;
	int buttonHeight = 70;
	Iterator<ProgramActionMirror*> it = p->getActions()->getIterator();
	while (it.hasNext()) {
		ProgramActionMirror* curr = it.next();
		ProgramDisplayActionButton* actionButton = new ProgramDisplayActionButton(actionButtonContainer_, ANCHOR_NORTHWEST, { 10, yDisp }, { actionButtonContainer_->getBounds().w - 20, buttonHeight }, *curr, index);
		actionButtonContainer_->addObject(actionButton);
		yDisp += buttonHeight + 10;
		index++;
	}

	SDL_Color c = p->getOwner()->getColor();
	SDL_SetTextureColorMod(_program_core_100px, c.r, c.g, c.b);

	actionButtonContainer_->setDimensions({ bounds_.w, yDisp });
	this->setDimensions({ bounds_.w, programInfoContainer_->getBounds().h + 10 + actionButtonContainer_->getBounds().h });
	resetBounds();
}

ProgramMirror* ProgramDisplayContainer::getCurrProg() {
	return currProg_;
}