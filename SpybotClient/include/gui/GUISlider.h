#ifndef GUISLIDER_H
#define GUISLIDER_H

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class GUISlider : public GUIObject {
public:
	GUISlider(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, void(*func) (float));
	virtual ~GUISlider();

	void setTransparency(int);
	bool mouseDown();
	bool mouseUp();
	void draw();
	void tick(int);
	void resetBounds();
protected:
private:
	SDL_Rect sliderBounds;
	int sliderVal;
	void(*func) (float);
};

#endif // GUISLIDER_H
