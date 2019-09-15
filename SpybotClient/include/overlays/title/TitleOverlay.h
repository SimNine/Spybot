#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class TitleOverlay : public GUIContainer {
public:
	TitleOverlay();
	virtual ~TitleOverlay();

	void tick(int);
	void draw();
	bool mouseDown();
protected:
private:
	//LinkedList<GUITexture*>* sprites;

	int tickCount;

	int timingCompany;
	int timingTitle;
	int timingSubtitle;
	int spanTiming;

	void advance();
	void endTitles();
};