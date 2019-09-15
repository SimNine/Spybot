#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class Notification;

class NotifyOverlay : public GUIContainer {
public:
	NotifyOverlay();
	virtual ~NotifyOverlay();
	void draw();
	void tick(int);

	void addNotification(std::string str);
protected:
private:
	LinkedList<Notification*>* notificationList_;
	int pushOffset;
	const int defaultDuration_ = 5000;
	const int defaultTextSize_ = 30;
};