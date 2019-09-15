#pragma once

#include "Standard.h"
#include "LinkedList.h"

class GUIContainer;
class GUIEffect;

class GUIObject {
public:
	GUIObject(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims);
	virtual ~GUIObject();

	SDL_Rect getBounds();
	void setDisplacement(Coord disp);
	Coord getDisplacement();
	void setDimensions(Coord dims);
	Coord getDimensions();

	bool isMouseOver();
	bool isVisible();
	bool isClickable();
	void setPressed(bool pressed);
	int getTransparency();

	void processEffects(int ms);
	void addEffect(GUIEffect* e);
	void removeAllEffects(bool finishFirst);

	void recomputePosition();

	// these to be overridden
	virtual void setTransparency(int) = 0;
	virtual bool mouseDown() = 0;
	virtual bool mouseUp() = 0;
	virtual void draw() = 0;
	virtual void tick(int millisec) = 0;
	virtual void resetBounds() = 0;
protected:
	void drawBounds();

	Coord displacement_;
	ANCHOR anchor_;
	SDL_Rect bounds_;
	GUIContainer* parent_;
	bool pressed_;

	int currAlpha_;
private:
	LinkedList<GUIEffect*>* effectList_;
	int getXAnchor();
	int getYAnchor();
};