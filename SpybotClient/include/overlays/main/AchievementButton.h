#pragma once

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class AchievementButton : public GUIObject {
public:
	AchievementButton(GUIContainer* parent, ANCHOR anchor, Coord displacement, ACHIEVEMENT achievement);
	virtual ~AchievementButton();

	void setTransparency(int);
	bool mouseDown();
	bool mouseUp();
	void draw();
	void tick(int);
	void resetBounds();
protected:
private:
	ACHIEVEMENT achievement_;
	Uint8 mouseoverFade_;
};