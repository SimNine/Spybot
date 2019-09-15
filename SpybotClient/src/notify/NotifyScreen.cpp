#include "Standard.h"
#include "NotifyScreen.h"

#include "Global.h"
#include "GUITexture.h"
#include "ResourceLoader.h"

NotifyScreen::NotifyScreen()
	: GUIContainer(ANCHOR_NORTHWEST, { 0, 0 }, { SCREEN_WIDTH, SCREEN_HEIGHT }, NULL, NULL)
{
	notificationList_ = new LinkedList<std::string*>();
	notificationTiming_ = new LinkedList<int*>();
	pushOffset = 0;
}

NotifyScreen::~NotifyScreen()
{
	//dtor
}

void NotifyScreen::draw()
{
	// set up constants
	int currTime = -1;
	SDL_Texture* currStr = NULL;
	int yOffset = -5;
	SDL_Rect strBounds;
	
	// for each notification
	for (int i = 0; i < notificationList_->getLength(); i++)
	{
		// create texture, set bounds
		currStr = loadString(*notificationList_->getObjectAt(i), FONT_BOLD, textSize, { 255, 255, 255, 255 });
		SDL_QueryTexture(currStr, NULL, NULL, &strBounds.w, &strBounds.h);
		yOffset -= strBounds.h;
		strBounds.x = SCREEN_WIDTH - strBounds.w - 5;
		strBounds.y = SCREEN_HEIGHT + yOffset + pushOffset;

		// set fade
		currTime = *notificationTiming_->getObjectAt(i);
		if (currTime < 1000)
			SDL_SetTextureAlphaMod(currStr, currTime / 4);

		// render and destroy
		SDL_RenderCopy(gRenderer, currStr, NULL, &strBounds);
		SDL_DestroyTexture(currStr);
	}
}

void NotifyScreen::tick(int ms)
{
	// update the push offset
	if (pushOffset > 0)
		pushOffset -= (ms/20 + 1);
	if (pushOffset < 0)
		pushOffset = 0;

	// update the time for each notification
	for (int i = 0; i < notificationTiming_->getLength(); i++)
		*notificationTiming_->getObjectAt(i) -= ms;

	// check for dead notifications
	while (notificationTiming_->getLength() > 0 && *notificationTiming_->getLast() <= 0)
	{
		delete notificationList_->removeLast();
		delete notificationTiming_->removeLast();
	}
}

void NotifyScreen::addNotification(std::string str)
{
	notificationList_->addFirst(new std::string(str));
	notificationTiming_->addFirst(new int(5000));
	pushOffset += textSize;
}