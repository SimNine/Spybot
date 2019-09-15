#pragma once

#include "Standard.h"
#include "Animation.h"

class AnimationTileFade : public Animation
{
public:
	AnimationTileFade(Coord pos, int time, Uint8 r, Uint8 g, Uint8 b);
	~AnimationTileFade();

	bool isDead();
	void draw(Coord pos);
	void tick(int ms);
private:
	Uint8 r_;
	Uint8 g_;
	Uint8 b_;
	int time_;
};