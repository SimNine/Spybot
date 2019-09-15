#pragma once

#include "Standard.h"

class GlowSpeck {
public:
	GlowSpeck();
	virtual ~GlowSpeck();
	void draw();
	void tick(int);

	Coord getPos();
	int getRadius();
	bool getTransparent();
protected:
private:
	int radius_;

	Uint8 rCol;
	Uint8 gCol;
	Uint8 bCol;

	float xPos, yPos;
	float xVel, yVel;
	float xAcc, yAcc;

	float aPos;
	float aVel;
	float aAcc;
};