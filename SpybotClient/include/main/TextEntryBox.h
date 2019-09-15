#pragma once

#include "GUIContainer.h"

class GUIButton;

class TextEntryBox : public GUIContainer
{
public:
	TextEntryBox(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, std::string prompt, void(*onOk) (void), void(*onCancel) (void));
	~TextEntryBox();

	void draw();

	std::string getContents();
	void clearContents();
	void addChar(char c);
private:
	std::string promptText_;
	std::string contentText_;
	
	GUIButton* okButton_;
	GUIButton* cancelButton_;

	const int textSize_ = 40;
};

