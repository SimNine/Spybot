#pragma once

#include "Standard.h"
#include "AnimationParticle.h"

class AnimationParticleSelectRing : public AnimationParticle {
public:
	AnimationParticleSelectRing(int time, Coord dims, SDL_Color col);
	virtual ~AnimationParticleSelectRing();

	void draw(Coord pos);
	void tick(int ms);
	bool isExpired();
protected:
private:
	Coord dims_;
	SDL_Color col_;
};