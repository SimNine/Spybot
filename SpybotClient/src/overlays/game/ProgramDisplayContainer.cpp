#include "Standard.h"
#include "ProgramDisplayContainer.h"

#include "Global.h"
#include "Program.h"
#include "ResourceLoader.h"
#include "GameOverlay.h"
#include "Data.h"
#include "MiscUtil.h"
#include "ProgramAction.h"
#include "Client.h"
#include "Player.h"
#include "Game.h"
#include "GUITexture.h"
#include "Client.h"
#include "ClientMirror.h"
#include "ProgramDisplayActionButton.h"
#include "ConnectionManager.h"

ProgramDisplayContainer::ProgramDisplayContainer(ANCHOR a, Coord disp, Coord dims, GUIContainer* par)
	: GUIContainer(par, a, disp, dims, _color_bkg_standard) {
	currProg_ = NULL;

	iconBacking_ = new GUITexture(this, ANCHOR_NORTHEAST, { -5, 5 }, { 150, 150 }, _program_core_100px);
	this->addObject(iconBacking_);

	icon_ = NULL;

	nameText_ = NULL;

	ownerText_ = new GUITexture(this, ANCHOR_NORTHWEST, { 5, 5 }, "Owner:", 30);
	this->addObject(ownerText_);

	healthText_ = new GUITexture(this, ANCHOR_NORTHWEST, { 5, 40 }, "Health:", 30);
	this->addObject(healthText_);

	movesText_ = new GUITexture(this, ANCHOR_NORTHWEST, { 5, 75 }, "Moves:", 30);
	this->addObject(movesText_);

	actionsText_ = new GUITexture(this, ANCHOR_NORTHWEST, { 5, 110 }, "Actions:", 30);
	this->addObject(actionsText_);

	descText_ = NULL;

	actionButtons_ = new LinkedList<ProgramDisplayActionButton*>();
}

ProgramDisplayContainer::~ProgramDisplayContainer() {
	//dtor
}

void ProgramDisplayContainer::draw() {
	// check for updated selected program
	if (_client->getPlayer()->getSelectedProgram() != currProg_)
		setCurrProg(_client->getPlayer()->getSelectedProgram());

	// draw bounds
	if (_debug >= DEBUG_NORMAL)
		drawBounds();

	// get the current program
	if (currProg_ == NULL)
		return;

	// draw all the permanent stuff
	GUIContainer::drawBkg();
	GUIContainer::drawContents();

	// draw dynamic stuff
	SDL_Rect bds;

	ClientMirror* cMirr = _connectionManager->getClientMirrorByPlayerID(currProg_->getOwner()->getPlayerID());
	bds = ownerText_->getBounds();
	bds.x += bds.w + 5;
	if (cMirr != NULL)
		drawString(cMirr->name_, FONT_NORMAL, 30, _color_white, bds);
	else
		drawString("AI", FONT_NORMAL, 30, _color_white, bds);

	bds = healthText_->getBounds();
	bds.x += bds.w + 5;
	drawString(to_string(currProg_->getHealth()) + "/" + to_string(currProg_->getMaxHealth()), FONT_NORMAL, 30, _color_white, bds);

	bds = movesText_->getBounds();
	bds.x += bds.w + 5;
	drawString(to_string(currProg_->getMoves()) + "/" + to_string(currProg_->getMaxMoves()), FONT_NORMAL, 30, _color_white, bds);

	bds = actionsText_->getBounds();
	bds.x += bds.w + 5;
	drawString(to_string(currProg_->getActionsLeft()) + "/" + to_string(1), FONT_NORMAL, 30, _color_white, bds);
}

void ProgramDisplayContainer::setCurrProg(Program* p) {
	if (icon_ != NULL) {
		this->removeObject(icon_);
		icon_ = NULL;
		delete icon_;
	}

	if (nameText_ != NULL) {
		this->removeObject(nameText_);
		nameText_ = NULL;
		delete nameText_;
	}

	while (actionButtons_->getLength() > 0) {
		ProgramDisplayActionButton* curr = actionButtons_->poll();
		this->removeObject(curr);
		delete curr;
	}

	//if (descText_ != NULL)
	//{
	//this->removeObject(descText_);
	//delete descText_;
	//}

	if (p != NULL) {
		icon_ = new GUITexture(this, ANCHOR_NORTHEAST, { -20, 5 }, { 135, 135 }, _program_icons[p->getType()], false);
		this->addObject(icon_);

		nameText_ = new GUITexture(this, ANCHOR_NORTHEAST, { -5, 155 }, p->getName(), 38);
		this->addObject(nameText_);

		//descText_ = new GUITexture(ANCHOR_NORTHWEST, { 5, 105 }, p->getDescription(), 20, this);
		//this->addObject(descText_);

		int yDisp = 0;
		int index = 0;
		int buttonHeight = 70;
		Iterator<ProgramAction*> it = p->getActions()->getIterator();
		while (it.hasNext()) {
			ProgramAction* curr = it.next();
			ProgramDisplayActionButton* actionButton = new ProgramDisplayActionButton(ANCHOR_NORTHEAST,
			{ -10, 200 + yDisp },
			{ bounds_.w - 20, buttonHeight },
				this, curr, index);
			actionButtons_->addFirst(actionButton);
			this->addObject(actionButton);
			yDisp += buttonHeight + 10;
			index++;
		}

		SDL_Color c = p->getOwner()->getColor();
		SDL_SetTextureColorMod(_program_core_100px, c.r, c.g, c.b);

		this->setDimensions({ bounds_.w, 200 + yDisp });
	}

	currProg_ = p;
	resetBounds();
}