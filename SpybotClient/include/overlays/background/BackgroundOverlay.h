#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class GlowSpeck;
class PromptBox;
class GUIButton;
class GUITextbox;

class BackgroundOverlay : public GUIContainer {
public:
	BackgroundOverlay();
	virtual ~BackgroundOverlay();

	void draw();
	void tick(int);
	void setMode(BKGMODE);
protected:
private:
	BKGMODE mode_;
	int textBkgDisplacement_;
	LinkedList<GlowSpeck*>* glowList_;
};