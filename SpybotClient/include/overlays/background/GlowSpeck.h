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

	Uint8 rCol_;
	Uint8 gCol_;
	Uint8 bCol_;

	float xPos_, yPos_;
	float xVel_, yVel_;
	float xAcc_, yAcc_;

	float aPos_;
	float aVel_;
	float aAcc_;
};