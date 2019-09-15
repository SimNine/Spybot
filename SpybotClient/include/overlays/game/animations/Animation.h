#pragma once

#include "Standard.h"
#include "LinkedList.h"

class AnimationParticle;

class Animation {
public:
	Animation(Coord pos, ANIMTYPE type);
	virtual ~Animation();

	virtual void draw(Coord pos);
	virtual void tick(int ms);
	virtual bool isDead();

	Coord getPos();
protected:
	LinkedList<AnimationParticle*>* particleList_;
private:
	Coord pos_;
	ANIMTYPE type_;
	int time_;
};