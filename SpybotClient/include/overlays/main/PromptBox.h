#pragma once

#include "GUIContainer.h"

class GUIButton;
class GUITextbox;

class PromptBox : public GUIContainer {
public:
	PromptBox(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, std::string prompt, void(*onOk) (void), void(*onCancel) (void));
	~PromptBox();

	void draw();

	std::string getContents();
	void clearContents();
	void addChar(char c);

	GUITextbox* getTextbox();
private:
	std::string promptText_;
	GUITextbox* entryField_;
	GUIButton* okButton_;
	GUIButton* cancelButton_;

	const int textSize_ = 40;
};

