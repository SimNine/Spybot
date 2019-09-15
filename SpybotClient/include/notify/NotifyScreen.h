#ifndef NOTIFYSCREEN_H
#define NOTIFYSCREEN_H

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class Notification;

class NotifyScreen : public GUIContainer {
public:
	NotifyScreen();
	virtual ~NotifyScreen();
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

#endif // NOTIFYSCREEN_H
