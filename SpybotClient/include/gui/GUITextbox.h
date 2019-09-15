#pragma once

#include "GUIObject.h"

class GUIButton;

class GUITextbox : public GUIObject {
public:
	GUITextbox(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, int maxChars, bool censored);
	~GUITextbox();

	void draw();
	bool mouseDown();
	bool mouseUp();
	void tick(int ms);
	void setTransparency(int alpha);
	void resetBounds();

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

