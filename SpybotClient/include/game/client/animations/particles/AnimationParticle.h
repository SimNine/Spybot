#pragma once

#include "Standard.h"

class AnimationParticle
{
public:
	AnimationParticle(int type);
	~AnimationParticle();

	virtual void draw(Coord pos) = 0;
	virtual void tick(int ms) = 0;
	virtual bool isExpired() = 0;
protected:
	int time_;
	int type_;
};