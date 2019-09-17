#include "Standard.h"
#include "ProgramInventoryDisplay.h"

#include "ResourceLoader.h"
#include "GUITexture.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Client.h"
#include "ClientMirror.h"
#include "GUIButtonParamaterized.h"
#include "GameOverlay.h"
#include "Message.h"
#include "PlayerMirror.h"
#include "ConnectionManager.h"
#include "ProgramPreset.h"
#include "MapOverlay.h"
#include "GameOverlay.h"

ProgramInventoryDisplay::ProgramInventoryDisplay(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims)
	: GUIContainer(parent, a, disp, dims, _color_bkg_standard) {
	resetBounds();

	this->addObject(new GUITexture(this, ANCHOR_NORTHWEST, { 20, 10 }, "PROGRAMS", 40));

	this->addObject(new GUITexture(this, ANCHOR_NORTHWEST, { iconCol_, 60 }, "ICON", 30));
	this->addObject(new GUITexture(this, ANCHOR_NORTHWEST, { nameCol_, 60 }, "NAME", 30));
	this->addObject(new GUITexture(this, ANCHOR_NORTHWEST, { numCol_, 60 }, "NUM", 30));
	this->addObject(new GUITexture(this, ANCHOR_NORTHWEST, { sizeCol_, 60 }, "SIZE", 30));
	this->addObject(new GUITexture(this, ANCHOR_NORTHWEST, { speedCol_, 60 }, "SPEED", 30));

	progContainer_ = new GUIContainer(this, ANCHOR_SOUTHWEST, { 20, -40 }, (getDimensions() - Coord{60, 140}), _color_bkg_standard);
	this->addObject(progContainer_);

	this->addObject(new GUIButtonParamaterized<ProgramInventoryDisplay*>(this, ANCHOR_SOUTHEAST, { -10, -10 }, { 30, 30 }, _arrow_down,
		[](ProgramInventoryDisplay* disp) {
		int newIndex = disp->getPageScroll() + 1;
		if (newIndex < 0)
			newIndex = 0;
		if (newIndex >= PROGRAM_NUM_PROGTYPES)
			newIndex = PROGRAM_NUM_PROGTYPES - 1;
		disp->setPageScroll(newIndex);
		disp->updateContents();
		}, 
		this));
	this->addObject(new GUIButtonParamaterized<ProgramInventoryDisplay*>(this, ANCHOR_SOUTHEAST, { -10, -50 }, { 30, 30 }, _arrow_up,
		[](ProgramInventoryDisplay* disp) {
		int newIndex = disp->getPageScroll() - 1;
		if (newIndex < 0)
			newIndex = 0;
		if (newIndex >= PROGRAM_NUM_PROGTYPES)
			newIndex = PROGRAM_NUM_PROGTYPES - 1;
		disp->setPageScroll(newIndex);
		disp->updateContents();
		},
		this));

	this->addObject(new GUIButton(this, ANCHOR_SOUTHWEST, { 10, -10 }, "MINIMIZE", 20,
		[]() {
		if (_overlayStack->contains(_mapOverlay))
			_mapOverlay->hideProgInv();
		if (_overlayStack->contains(_gameOverlay))
			_gameOverlay->hideProgInv();
	}));
}

ProgramInventoryDisplay::~ProgramInventoryDisplay() {
	//dtor
}

void ProgramInventoryDisplay::updateContents() {
	// empty program container contents
	while (progContainer_->getContents()->getLength() > 0) {
		delete progContainer_->getContents()->poll();
	}

	// collect the enums of all programs that the user has at least one of
	int row = 0;
	LinkedList<int*> foundProgs;
	for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
		if (_client->getMyClientMirror()->ownedProgs_[i] > 0)
			foundProgs.addLast(new int(i));
	}

	// change the pagescroll if it would result in not showing any programs
	if (pageScroll_ >= foundProgs.getLength()) {
		if (foundProgs.getLength() == 0)
			pageScroll_ = 0;
		else
			pageScroll_ = foundProgs.getLength() - 1;
	}

	for (int x = pageScroll_; x < foundProgs.getLength(); x++) {
		int i = *foundProgs.getObjectAt(x);

		if (i == PROGRAM_NONE || i == PROGRAM_CUSTOM)
			continue;

		if (_client->getMyClientMirror()->ownedProgs_[i] <= 0)
			continue;

		// get the preset
		ProgramPreset pp = getProgramPreset((PROGRAM)i);
		
		// create the icon / button
		progContainer_->addObject(new GUIButtonParamaterized<PROGRAM>(progContainer_, ANCHOR_NORTHWEST, { iconCol_ - 20, row }, { 30, 30 },
			_program_icons_paths[i], [](PROGRAM p) {
			// check for associated player
			if (_client->getPlayer() == NULL) {
				log("CLIENT ERR: clicked programselectbutton without a player\n");
				return;
			}

			Message m;
			m.type = MSGTYPE_PLACEPROG;
			m.progType = p;
			m.pos = _client->getPlayer()->getSelectedTile();
			m.playerID = _client->getPlayer()->getPlayerID();
			_connectionManager->sendMessage(m);
		}, (PROGRAM)i));

		// create the name
		progContainer_->addObject(new GUITexture(progContainer_, ANCHOR_NORTHWEST, { nameCol_ - 20, row }, pp.name_, 30));

		// create the num
		std::string str = to_string(_client->getMyClientMirror()->ownedProgs_[i]);
		progContainer_->addObject(new GUITexture(progContainer_, ANCHOR_NORTHWEST, { numCol_ - 20, row }, str, 30));

		// create the size
		progContainer_->addObject(new GUITexture(progContainer_, ANCHOR_NORTHWEST, { sizeCol_ - 20, row }, to_string(pp.maxHealth_), 30));

		// create the speed
		progContainer_->addObject(new GUITexture(progContainer_, ANCHOR_NORTHWEST, { speedCol_ - 20, row }, to_string(pp.maxMoves_), 30));

		// increment the row
		row += 30;

		// if no more rows will fit, exit this loop
		if (row + 30 >= progContainer_->getDimensions().y)
			break;
	}

	// destroy list of found programs
	while (foundProgs.getLength() > 0) {
		delete foundProgs.poll();
	}
}

void ProgramInventoryDisplay::resetBounds() {
	GUIContainer::resetBounds();
}

int ProgramInventoryDisplay::getPageScroll() {
	return pageScroll_;
}

void ProgramInventoryDisplay::setPageScroll(int amt) {
	pageScroll_ = amt;
}