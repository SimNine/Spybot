#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"
#include "GUIEffectFade.h"

class PopupNotification;
class PopupAchievement;

class NotifyOverlay : public GUIContainer {
public:
	NotifyOverlay();
	virtual ~NotifyOverlay();
	void draw();
	void tick(int);

	void addNotification(std::string str);
	void addAchievement(ACHIEVEMENT a);
protected:
private:
	LinkedList<PopupAchievement*>* achievementList_;
	LinkedList<PopupNotification*>* notificationList_;
	int pushOffset_;
	const int defaultDuration_ = 5000;
	const int defaultTextSize_ = 30;
};