#ifndef ANIMATIONATTACKSQUARESPECK_H
#define ANIMATIONATTACKSQUARESPECK_H

#include "Standard.h"
#include "AnimationParticle.h"

class AnimationParticleSquareSpeck : public AnimationParticle
{
public:
	AnimationParticleSquareSpeck(int time, Coord disp, Coord vel);
    virtual ~AnimationParticleSquareSpeck();

	void draw(Coord pos);
	void tick(int ms);
	bool isExpired();
protected:
private:
	Coord disp_;
	Coord vel_;
};

#endif // GAMESCREEN_H
