#pragma once

#include "GUIContainer.h"

class GUIButton;

class GUITextbox : public GUIContainer {
public:
	GUITextbox(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, int maxChars, bool censored);
	~GUITextbox();

	void draw();
	bool mouseDown();
	bool mouseUp();
	void tick(int ms);

	std::string getContents();
	void clearContents();
	void addChar(char c);
private:
	std::string contentText_;
	int textSize_;
	int maxChars_;
	int cursorFade_;
	bool censored_;
};

