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

UserDisplay::UserDisplay(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, User* user)
	: GUIContainer(parent, anchor, disp, dims, _color_bkg_standard) {
	if (user == NULL) {
		GUIButton* createButton_ = new GUIButton(this, ANCHOR_SOUTH, { 0, -50 }, "Create Savefile", 30, [] () {
			char temp[20];
			_itoa_s(randInt(), temp, 20);
			std::string text = "user" + std::string(temp) + "\n" + "userpass";
			Message m;
			m.type = MSGTYPE_CREATEUSER;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, text.c_str(), DEFAULT_MSG_TEXTSIZE);
			_connectionManager->sendMessage(m);
		});
		this->addObject(createButton_);
	} else {
		GUITexture* username_ = new GUITexture(this, ANCHOR_NORTH, { 0, 30 }, user->username_, 30);
		this->addObject(username_);
		GUITexture* wins_ = new GUITexture(this, ANCHOR_NORTH, { 0, 60 }, "Wins: " + to_string(user->numWins_), 30);
		this->addObject(wins_);
		GUITexture* losses_ = new GUITexture(this, ANCHOR_NORTH, { 0, 90 }, "Losses: " + to_string(user->numLosses_), 30);
		this->addObject(losses_);

		GUIButtonParamaterized<User*>* deleteButton_ = new GUIButtonParamaterized<User*>(this, ANCHOR_SOUTH, { 0, -50 }, "Delete Savefile", 30,
			[] (User* u) {
			printf(u->username_.c_str());
		}, user);
		this->addObject(deleteButton_);

		GUIButtonParamaterized<User*>* loadButton_ = new GUIButtonParamaterized<User*>(this, ANCHOR_SOUTH, { 0, -100 }, "Load Savefile", 30,
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