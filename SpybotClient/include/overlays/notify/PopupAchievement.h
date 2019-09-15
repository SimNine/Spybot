#pragma once

#include "Standard.h"

class PopupAchievement {
public:
	PopupAchievement(ACHIEVEMENT a, int duration);
	virtual ~PopupAchievement();

	void tick(int ms);
	void setTimeLeft(int ms);
	int getTimeLeft();

	ACHIEVEMENT getAchievement();
protected:
private:
	ACHIEVEMENT achievement_;
	int timeLeft_;
};