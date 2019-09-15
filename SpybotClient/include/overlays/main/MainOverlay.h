#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class GlowSpeck;
class PromptBox;
class GUIButton;
class GUITextbox;

class MainOverlay : public GUIContainer {
public:
	MainOverlay();
	virtual ~MainOverlay();

	void draw();
	void tick(int);

	void toggleOptions();

	void showIPEntry();
	void hideIPEntry();
	void submitIPEntry();

	void loginShow();
	void loginHide();
	void loginCreate();
	void loginSubmit();
	void loginClear();

	void hideMainContainer();
	void showMainContainer();

	void keyPress(char c);
protected:
private:
	GUIContainer* optionsContainer_;
	GUIContainer* mainContainer_;

	GUIContainer* IPEntryContainer_;
	PromptBox* IPEntryBox_;

	GUIContainer* loginEntryContainer_;
	GUIButton* loginCancelButton_;
	GUIButton* loginCreateButton_;
	GUIButton* loginSubmitButton_;
	GUITextbox* loginUsername_;
	GUITextbox* loginPassword_;
};