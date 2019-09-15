#include "Standard.h"
#include "AnimationTileFade.h"

#include "Animation.h"
#include "Global.h"

AnimationTileFade::AnimationTileFade(Coord pos, int time, Uint8 r, Uint8 g, Uint8 b)
	: Animation(pos, ANIMTYPE_TILEFADE)
{
	r_ = r;
	g_ = g;
	b_ = b;
	time_ = time;
}

AnimationTileFade::~AnimationTileFade()
{

}

void AnimationTileFade::tick(int ms)
{
	time_ -= ms;
}

void AnimationTileFade::draw(Coord pos)
{
	SDL_Rect coord;
	SDL_QueryTexture(_program_core, NULL, NULL, &coord.w, &coord.h);
	if (time_ <= 255)
		SDL_SetTextureAlphaMod(_program_core, time_);
	else
		SDL_SetTextureAlphaMod(_program_core, 255);
	SDL_SetTextureColorMod(_program_core, r_, g_, b_);
	coord.x = pos.x - coord.w / 2 - 2;
	coord.y = pos.y - coord.h / 2 - 2;
	SDL_RenderCopy(_renderer, _program_core, NULL, &coord);
}

bool AnimationTileFade::isDead()
{
	return (time_ <= 0);
}