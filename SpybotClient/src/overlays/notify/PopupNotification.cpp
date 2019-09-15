#include "Standard.h"
#include "PopupNotification.h"

PopupNotification::PopupNotification(std::string content, int duration, int size) {
	content_ = content;
	timeLeft_ = duration;
	textSize_ = size;
}

PopupNotification::~PopupNotification() {
	//dtor
}

void PopupNotification::tick(int ms) {
	timeLeft_ -= ms;
}

void PopupNotification::setTimeLeft(int ms) {
	timeLeft_ = ms;
}

int PopupNotification::getTimeLeft() {
	return timeLeft_;
}

std::string PopupNotification::getContent() {
	return content_;
}

int PopupNotification::getSize() {
	return textSize_;
}

void PopupNotification::setSize(int size) {
	textSize_ = size;
}