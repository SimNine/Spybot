#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class Node;
class GUIButton;
class ProgramInventoryDisplay;
class GUITexture;
class UserDisplay;

class LocalLoginOverlay : public GUIContainer {
public:
	LocalLoginOverlay();
	virtual ~LocalLoginOverlay();

	void draw();
	void tick(int);

	void refreshUsers();
protected:
private:
	GUITexture* saveContainerTitle_;
	GUIContainer* saveContainer_;

	UserDisplay* user1Container_;
	UserDisplay* user2Container_;
	UserDisplay* user3Container_;

	GUIButton* backButton_;
};