#pragma once

#include "GUIContainer.h"
#include "GUIButtonParamaterized.h"

class User;
class GUIButton;
class GUITexture;

class UserDisplay : public GUIContainer {
public:
	UserDisplay(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, User* user);
	~UserDisplay();

	void draw();
	void tick(int ms);
private:
	User* user_;
	GUITexture* wins_;
	GUITexture* losses_;
	GUITexture* username_;

	GUIButton* createButton_;
	GUIButtonParamaterized<User*>* deleteButton_;
	GUIButtonParamaterized<User*>* loadButton_;
};

