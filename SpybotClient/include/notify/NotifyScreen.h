#ifndef NOTIFYSCREEN_H
#define NOTIFYSCREEN_H

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class GUITexture;

class NotifyScreen : public GUIContainer
{
public:
	NotifyScreen();
	virtual ~NotifyScreen();
	void draw();
	void tick(int);

	void addNotification(std::string str);
protected:
private:
	LinkedList<std::string*>* notificationList_;
	GUITexture* currTex_;
	int texWidth_;
	int timeLeft_;
};

#endif // NOTIFYSCREEN_H
