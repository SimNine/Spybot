#include "Standard.h"
#include "PopupAchievement.h"

#include "ResourceLoader.h"

PopupAchievement::PopupAchievement(ACHIEVEMENT a, int duration) {
	timeLeft_ = duration;
	achievement_ = a;
}

PopupAchievement::~PopupAchievement() {
	//dtor
}

void PopupAchievement::tick(int ms) {
	timeLeft_ -= ms;
}

void PopupAchievement::setTimeLeft(int ms) {
	timeLeft_ = ms;
}

int PopupAchievement::getTimeLeft() {
	return timeLeft_;
}

ACHIEVEMENT PopupAchievement::getAchievement() {
	return achievement_;
}