#ifndef PLAYERDISPLAYCONTAINER_H
#define PLAYERDISPLAYCONTAINER_H

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

#endif // PLAYERDISPLAYCONTAINER_H
