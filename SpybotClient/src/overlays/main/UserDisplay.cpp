#include "Standard.h"
#include "UserDisplay.h"

#include "Global.h"
#include "GUIButton.h"
#include "Message.h"
#include "MiscUtil.h"
#include "ConnectionManager.h"
#include "ResourceLoader.h"
#include "User.h"
#include "GUITexture.h"
#include "GUIButtonParamaterized.h"
#include "MainOverlay.h"
#include "Server.h"
#include "TimedEvent.h"

UserDisplay::UserDisplay(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, User* user)
	: GUIContainer(parent, anchor, disp, dims, _color_bkg_standard) {
	if (user == NULL) {
		GUIButton* createButton_ = new GUIButton(this, ANCHOR_SOUTH, { 0, -50 }, "CREATE", 30, [] () {
			_mainOverlay->hideLocalLoginContainer(1000);
			_mainOverlay->showLocalUsernameEntry(1000);
		});
		this->addObject(createButton_);
	} else {
		GUITexture* username_ = new GUITexture(this, ANCHOR_NORTH, { 0, 30 }, user->username_, 30);
		this->addObject(username_);
		GUITexture* credits_ = new GUITexture(this, ANCHOR_NORTH, { 0, 60 }, "CREDITS: " + to_string(user->numCredits_), 30);
		this->addObject(credits_);
		int numCompleted = 0;
		// TODO: fix this. is hardcoded to only work for classic, not nightfall or freeform
		for (int i = 0; i < NUM_LEVELS_CLASSIC; i++) { 
			if (user->campaignClassic_[i])
				numCompleted++;
		}
		GUITexture* completion_ = new GUITexture(this, ANCHOR_NORTH, { 0, 90 }, "PROGRESS: " + to_string(numCompleted) + "/" + to_string(NUM_LEVELS_CLASSIC), 30);
		this->addObject(completion_);

		GUIButtonParamaterized<User*>* deleteButton_ = new GUIButtonParamaterized<User*>(this, ANCHOR_SOUTH, { 0, -30 }, "DELETE", 30,
			[] (User* u) {
			// TODO: find a way to do this that is consistent with the actual control flow
			_server->getUsers()->remove(u);
			log("CLIENT: deleted user " + u->username_);
			delete u;
			_server->saveUsers();
			_timedEvents->addLast(new TimedEvent(1, 
				[]() {
				_mainOverlay->refreshUsers();
			}));
		}, user);
		this->addObject(deleteButton_);

		GUIButtonParamaterized<User*>* loadButton_ = new GUIButtonParamaterized<User*>(this, ANCHOR_SOUTH, { 0, -60 }, "LOAD", 30,
			[] (User* u) {
			std::string text = u->username_ + "\n" + u->password_;
			Message m;
			m.type = MSGTYPE_LOGIN;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, text.c_str(), DEFAULT_MSG_TEXTSIZE);
			_connectionManager->sendMessage(m);
		}, user);
		this->addObject(loadButton_);
	}
}

UserDisplay::~UserDisplay() {

}

void UserDisplay::draw() {
	GUIContainer::drawBkg();
	GUIContainer::drawContents();
}

void UserDisplay::tick(int ms) {

}