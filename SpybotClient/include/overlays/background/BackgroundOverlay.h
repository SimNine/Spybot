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
protected:
private:
	int textBkgDisplacement_;
	LinkedList<GlowSpeck*>* glowList_;
};