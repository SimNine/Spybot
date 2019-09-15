#include "Standard.h"
#include "Animation.h"

#include "LinkedList.h"
#include "Global.h"
#include "AnimationParticle.h"
#include "AnimationParticleSquareSpeck.h"
#include "AnimationParticleSelectRing.h"
#include "Client.h"
#include "Game.h"
#include "Program.h"

Animation::Animation(Coord pos, ANIMTYPE type)
{
	pos_ = pos;
	type_ = type;
	particleList_ = new LinkedList<AnimationParticle*>();
}

Animation::~Animation()
{
	while (particleList_->getLength() > 0)
		delete particleList_->poll();
	delete particleList_;
}

void Animation::tick(int ms)
{
	Iterator<AnimationParticle*> it = particleList_->getIterator();
	while (it.hasNext())
		it.next()->tick(ms);

	int i = 0;
	while (i < particleList_->getLength())
	{
		AnimationParticle* currPart = particleList_->getObjectAt(i);
		if (currPart->isExpired())
		{
			particleList_->removeObjectAt(i);
			delete currPart;
		}
		else
			i++;
	}
}

void Animation::draw(Coord pos)
{
	Iterator<AnimationParticle*> it = particleList_->getIterator();
	while (it.hasNext())
		it.next()->draw(pos);
}

Coord Animation::getPos()
{
	return pos_;
}

bool Animation::isDead()
{
	return (particleList_->getLength() == 0);
}