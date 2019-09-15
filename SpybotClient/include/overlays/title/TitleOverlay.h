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

	int timingCompany_;
	int timingTitle_;
	int timingSubtitle_;
	int spanTiming_;

	void advance();
	void endTitles();
};