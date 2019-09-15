#pragma once

#include "Standard.h"
#include "Animation.h"

class AnimationParticle;

class AnimationAttack : public Animation {
public:
	AnimationAttack(Coord pos, int damage);
	~AnimationAttack();
private:
};