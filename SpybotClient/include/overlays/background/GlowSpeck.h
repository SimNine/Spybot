#pragma once

#include "Standard.h"

class GlowSpeck {
public:
	GlowSpeck(SDL_Color col);
	virtual ~GlowSpeck();
	void draw();
	void tick(int);

	Coord getPos();
	int getRadius();
	bool getTransparent();
protected:
private:
	int radius_;

	SDL_Color col_;

	float xPos_, yPos_;
	float xVel_, yVel_;
	float xAcc_, yAcc_;

	float aPos_;
	float aVel_;
	float aAcc_;
};