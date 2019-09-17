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
	int tickCount_;

	int timingUrf_;
	int timingGamelab_;
	int timingLego_;
	int timingTitle_;
	int timingSubtitle_;
	int spanFade_;
	int spanHold_;

	void endTitles();
};