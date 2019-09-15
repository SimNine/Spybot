#pragma once

#include "Standard.h"
#include "GUIObject.h"
#include "LinkedList.h"

class GUIContainer : public GUIObject {
public:
	GUIContainer(GUIContainer* parent, ANCHOR anch, Coord disp, Coord dims, SDL_Color col);

	virtual ~GUIContainer();
	SDL_Color getBackgroundCol();
	void setBackgroundCol(SDL_Color);
	void addObject(GUIObject*);
	void removeObject(GUIObject*);
	void addAllObjects(LinkedList<GUIObject*>*);
	LinkedList<GUIObject*>* getContents();
	void setPressed(bool);

	void resetBounds();
	void draw();
	bool mouseDown();
	bool mouseUp();
	void tick(int);
	void setTransparency(int);

	void incDisplacement(Coord);

	bool isMovable();
	void setMovable(bool);

protected:
	SDL_Color bkgCol_;
	void drawBkg();
	void drawContents();

	bool movable_;

	LinkedList<GUIObject*>* contents_;

private:
};