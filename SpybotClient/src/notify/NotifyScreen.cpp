#include "Standard.h"
#include "NotifyScreen.h"

#include "Global.h"
#include "GUITexture.h"

NotifyScreen::NotifyScreen()
	: GUIContainer(ANCHOR_NORTHWEST, { 0, 0 }, { SCREEN_WIDTH, SCREEN_HEIGHT }, NULL, NULL)
{
	notificationList_ = new LinkedList<std::string*>();
	timeLeft_ = 0;
	currTex_ = NULL;
	texWidth_ = 0;
}

NotifyScreen::~NotifyScreen()
{
	//dtor
}

void NotifyScreen::draw()
{
	if (currTex_ != NULL)
		currTex_->draw();
}

void NotifyScreen::tick(int ms)
{
	static const int offset = -25;

	if (timeLeft_ > 0)
	{
		timeLeft_ -= ms;

		if (timeLeft_ > 4000)
		{
			int newDisp = ((5000 - timeLeft_)*texWidth_) / 1000 - offset;
			currTex_->setBounds({ -newDisp, -75 }, { currTex_->getBounds()->w, currTex_->getBounds()->h });
		}
		else if (timeLeft_ < 1000)
		{
			int newDisp = (timeLeft_*texWidth_) / 1000 - offset;
			currTex_->setBounds({ -newDisp, -75 }, { currTex_->getBounds()->w, currTex_->getBounds()->h });
		}
		else
		{
			currTex_->setBounds({ -texWidth_ + offset, -75 }, { currTex_->getBounds()->w, currTex_->getBounds()->h });
		}
	}
	else
	{
		if (currTex_ != NULL)
		{
			delete currTex_;
			currTex_ = NULL;
			texWidth_ = 0;
		}

		if (notificationList_->getLength() > 0)
		{
			std::string* currStr = notificationList_->poll();
			currTex_ = new GUITexture(ANCHOR_SOUTHEAST, { 0, -75 }, *currStr, this);
			texWidth_ = currTex_->getBounds()->w;
			delete currStr;

			timeLeft_ = 5000 - notificationList_->getLength()*600;
			if (timeLeft_ < 2000)
				timeLeft_ = 2000;
		}
	}
}

void NotifyScreen::addNotification(std::string str)
{
	notificationList_->addLast(new std::string(str));
}