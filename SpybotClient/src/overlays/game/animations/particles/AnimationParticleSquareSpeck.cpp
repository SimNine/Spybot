#include "Standard.h"
#include "AnimationParticleSquareSpeck.h"

#include "Global.h"

AnimationParticleSquareSpeck::AnimationParticleSquareSpeck(int time, Coord disp, Coord vel)
	: AnimationParticle(0) {
	disp_ = disp;
	vel_ = vel;
	time_ = time;
}

AnimationParticleSquareSpeck::~AnimationParticleSquareSpeck() {
	//dtor
}

void AnimationParticleSquareSpeck::tick(int ms) {
	disp_.x += (int)(0.01*(double)vel_.x*(double)ms + 1);
	disp_.y += (int)(0.01*(double)vel_.y*(double)ms + 1);
	time_ -= ms;
}

void AnimationParticleSquareSpeck::draw(Coord pos) {
	SDL_Rect coord;
	SDL_QueryTexture(_particle_basic, NULL, NULL, &coord.w, &coord.h);
	SDL_SetTextureAlphaMod(_particle_basic, time_ / 4);
	coord.x = pos.x + disp_.x - coord.w / 2;
	coord.y = pos.y + disp_.y - coord.h / 2;
	SDL_RenderCopy(_renderer, _particle_basic, NULL, &coord);
}

bool AnimationParticleSquareSpeck::isExpired() {
	return (time_ < 0);
}