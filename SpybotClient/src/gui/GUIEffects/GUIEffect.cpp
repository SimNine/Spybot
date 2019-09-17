#include "Standard.h"
#include "GUIEffect.h"

GUIEffect::GUIEffect(int delay, int duration) {
	delayRemaining_ = delay;
	durationElapsed_ = 0;
	durationRemaining_ = duration;
	percentFinished_ = 0.0;
}

GUIEffect::~GUIEffect() {
	//dtor
}

void GUIEffect::tick(int ms) {
	if (delayRemaining_ > 0) {
		delayRemaining_ -= ms;
	} else {
		durationRemaining_ -= ms;
		durationElapsed_ += ms;
		percentFinished_ = (double)durationElapsed_ / (double)(durationElapsed_ + durationRemaining_);
	}
}

double GUIEffect::getPercentFinished() {
	return percentFinished_;
}

bool GUIEffect::isDead() {
	return (durationRemaining_ <= 0);
}

bool GUIEffect::isActive() {
	return (delayRemaining_ <= 0);
}