#pragma once

#include "Standard.h"

class PopupNotification {
public:
	PopupNotification(std::string content, int duration, int size);
	virtual ~PopupNotification();

	void tick(int ms);
	void setTimeLeft(int ms);
	int getTimeLeft();

	void setSize(int size);
	int getSize();

	std::string getContent();
protected:
private:
	std::string content_;

	int timeLeft_;
	int textSize_;
};