#pragma once

#include "GUIContainer.h"
#include "GUIButtonParamaterized.h"

class GUITextbox;

class PromptBox : public GUIContainer {
public:
	PromptBox(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, std::string prompt, void(*onOk) (PromptBox* thisObj), void(*onCancel) (PromptBox* thisObj));
	~PromptBox();

	void draw();

	std::string getContents();
	void clearContents();
	void addChar(char c);

	GUITextbox* getTextbox();
private:
	std::string promptText_;
	GUITextbox* entryField_;
	GUIButtonParamaterized<PromptBox*>* okButton_;
	GUIButtonParamaterized<PromptBox*>* cancelButton_;

	const int textSize_ = 40;
};

