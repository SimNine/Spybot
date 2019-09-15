#include "Standard.h"
#include "Notification.h"

Notification::Notification(std::string content, int duration, int size) {
	content_ = content;
	timeLeft_ = duration;
	textSize_ = size;
}

Notification::~Notification() {
	//dtor
}

void Notification::tick(int ms) {
	timeLeft_ -= ms;
}

void Notification::setTimeLeft(int ms) {
	timeLeft_ = ms;
}

int Notification::getTimeLeft() {
	return timeLeft_;
}

std::string Notification::getContent() {
	return content_;
}

int Notification::getSize() {
	return textSize_;
}

void Notification::setSize(int size) {
	textSize_ = size;
}