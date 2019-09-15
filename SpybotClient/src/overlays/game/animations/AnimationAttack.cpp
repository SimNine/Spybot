#include "Standard.h"
#include "AnimationAttack.h"

#include "Animation.h"
#include "AnimationParticleSquareSpeck.h"

AnimationAttack::AnimationAttack(Coord pos, int damage)
	: Animation(pos, ANIMTYPE_ACTION_ATTACK) {
	int numParticles = damage * 10;
	for (int i = 0; i < numParticles; i++)
		particleList_->addFirst(new AnimationParticleSquareSpeck(rand() % 1000, { 0, 0 }, { rand() % 129 - 64, rand() % 129 - 64 }));
}

AnimationAttack::~AnimationAttack() {
	//dtor
}