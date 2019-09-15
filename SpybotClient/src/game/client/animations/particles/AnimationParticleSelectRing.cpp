#include "Standard.h"
#include "AnimationParticleSelectRing.h"

#include "Global.h"

AnimationParticleSelectRing::AnimationParticleSelectRing(int time, Coord dims, SDL_Color col)
    : AnimationParticle(0)
{
	dims_ = dims;
	time_ = time;
	col_ = col;
}

AnimationParticleSelectRing::~AnimationParticleSelectRing()
{
    //dtor
}

void AnimationParticleSelectRing::tick(int ms)
{
	time_ -= ms;
}

void AnimationParticleSelectRing::draw(Coord pos)
{
	if (time_ > 250)
		return;

	if (time_ > 125)
		SDL_SetTextureAlphaMod(_particle_selectring, -(time_*2 - 500));
	if (time_ <= 125)
		SDL_SetTextureAlphaMod(_particle_selectring, time_*2);
	SDL_SetTextureColorMod(_particle_selectring, col_.r, col_.g, col_.b);
	SDL_Rect coord;
	coord.w = dims_.x;
	coord.h = dims_.y;
	coord.x = pos.x - coord.w / 2;
	coord.y = pos.y - coord.h / 2;
	coord.x -= 4;
	coord.y -= 4; // offset because a "tile" includes its right and lower boundary
	SDL_RenderCopy(_renderer, _particle_selectring, NULL, &coord);
}

bool AnimationParticleSelectRing::isExpired()
{
	return (time_ < 0);
}