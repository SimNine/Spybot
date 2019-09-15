#include "Standard.h"
#include "AnimationSelect.h"

#include "Animation.h"
#include "AnimationParticleSelectRing.h"

AnimationSelect::AnimationSelect(Coord pos, Uint8 r, Uint8 g, Uint8 b)
	: Animation(pos, ANIMTYPE_SELECT) {
	particleList_->addFirst(new AnimationParticleSelectRing(250, { 20, 20 }, { r, g, b, 255 }));
	particleList_->addFirst(new AnimationParticleSelectRing(375, { 40, 40 }, { r, g, b, 255 }));
	particleList_->addFirst(new AnimationParticleSelectRing(500, { 60, 60 }, { r, g, b, 255 }));
	particleList_->addFirst(new AnimationParticleSelectRing(625, { 80, 80 }, { r, g, b, 255 }));
	particleList_->addFirst(new AnimationParticleSelectRing(750, { 100, 100 }, { r, g, b, 255 }));
}

AnimationSelect::~AnimationSelect() {

}