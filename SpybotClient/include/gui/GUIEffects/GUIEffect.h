#pragma once

#include "Standard.h"

class GUIObject;

class GUIEffect {
public:
	GUIEffect(int delay, int duration);
	virtual ~GUIEffect();

	void tick(int millisec);
	double getPercentFinished();

	bool isDead();
	bool isActive();

	virtual void affectObject(GUIObject* object) = 0;
protected:
	int delayRemaining_;

	int durationRemaining_;
	int durationElapsed_;
	double percentFinished_;
};