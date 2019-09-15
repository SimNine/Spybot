#include "Standard.h"
#include "LocalLoginOverlay.h"

#include "Global.h"
#include "Data.h"
#include "GUIButton.h"
#include "GUITexture.h"
#include "BackgroundOverlay.h"
#include "MainOverlay.h"
#include "UserDisplay.h"
#include "LinkedList.h"
#include "Server.h"

LocalLoginOverlay::LocalLoginOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _SCREEN_WIDTH, _SCREEN_HEIGHT }, NULL) {
	// create title
	saveContainerTitle_ = new GUITexture(this, ANCHOR_CENTER, { 0, -190 }, "Select a Savefile", 80);
	this->addObject(saveContainerTitle_);

	// create container
	saveContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 640, 300 }, _color_bkg_standard);
	this->addObject(saveContainer_);

	// add back button
	backButton_ = new GUIButton(this, ANCHOR_SOUTHWEST, { 20, 0 }, "BACK", 50, [] () {
		_overlayStack->removeAll();
		_overlayStack->push(_backgroundOverlay);
		_overlayStack->push(_mainOverlay);
		_mainOverlay->loginHide();
	});
	this->addObject(backButton_);

	// nullify default containers
	user1Container_ = NULL;
	user2Container_ = NULL;
	user3Container_ = NULL;
}

LocalLoginOverlay::~LocalLoginOverlay() {
	//dtor
}

void LocalLoginOverlay::draw() {
	GUIContainer::drawContents();
}

void LocalLoginOverlay::tick(int ms) {
	// tick all GUIObjects
	GUIContainer::tick(ms);
}

void LocalLoginOverlay::refreshUsers() {
	// remove all previous contents
	saveContainer_->getContents()->removeAll();
	if (user1Container_ != NULL) {
		delete user1Container_;
		user1Container_ = NULL;
	}
	if (user2Container_ != NULL) {
		delete user2Container_;
		user2Container_ = NULL;
	}
	if (user3Container_ != NULL) {
		delete user3Container_;
		user3Container_ = NULL;
	}

	// populate container
	if (_server != NULL) {
		LinkedList<User*>* users = _server->getUsers();
		int xDisp = 10;
		user1Container_ = new UserDisplay(ANCHOR_NORTHWEST, { xDisp, 10 }, { 200, 280 }, saveContainer_, users->getObjectAt(0));
		saveContainer_->addObject(user1Container_);
		xDisp += 200 + 10;
		user2Container_ = new UserDisplay(ANCHOR_NORTHWEST, { xDisp, 10 }, { 200, 280 }, saveContainer_, users->getObjectAt(1));
		saveContainer_->addObject(user2Container_);
		xDisp += 200 + 10;
		user3Container_ = new UserDisplay(ANCHOR_NORTHWEST, { xDisp, 10 }, { 200, 280 }, saveContainer_, users->getObjectAt(2));
		saveContainer_->addObject(user3Container_);
	}
}