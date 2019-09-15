#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class PlayerDisplayContainer : public GUIContainer
{
public:
	PlayerDisplayContainer(ANCHOR anchor, Coord, Coord, GUIContainer*);
	virtual ~PlayerDisplayContainer();

	void draw();
protected:
private:
};