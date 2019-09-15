#include "Standard.h"
#include "NotifyOverlay.h"

#include "Global.h"
#include "GUITexture.h"
#include "ResourceLoader.h"
#include "Notification.h"

NotifyOverlay::NotifyOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _SCREEN_WIDTH, _SCREEN_HEIGHT }, NULL) {
	notificationList_ = new LinkedList<Notification*>();
	pushOffset = 0;
}

NotifyOverlay::~NotifyOverlay() {
	//dtor
}

void NotifyOverlay::draw() {
	// set up constants
	int currTime = -1;
	SDL_Texture* currStr = NULL;
	int yOffset = -5;
	SDL_Rect strBounds;

	// for each notification
	Iterator<Notification*> it = notificationList_->getIterator();
	while (it.hasNext()) {
		Notification* curr = it.next();

		// create texture, set bounds
		currStr = loadString(curr->getContent(), FONT_BOLD, curr->getSize(), { 255, 255, 255, 255 });
		SDL_QueryTexture(currStr, NULL, NULL, &strBounds.w, &strBounds.h);
		yOffset -= strBounds.h;
		strBounds.x = _SCREEN_WIDTH - strBounds.w - 5;
		strBounds.y = _SCREEN_HEIGHT + yOffset + pushOffset;

		// set fade
		currTime = curr->getTimeLeft();
		if (currTime < 1000)
			SDL_SetTextureAlphaMod(currStr, currTime / 4);

		// render and destroy
		SDL_RenderCopy(_renderer, currStr, NULL, &strBounds);
		SDL_DestroyTexture(currStr);
	}
}

void NotifyOverlay::tick(int ms) {
	// update the push offset
	if (pushOffset > 0)
		pushOffset -= (ms / 20 + 1);
	if (pushOffset < 0)
		pushOffset = 0;

	// update the time for each notification
	Iterator<Notification*> it = notificationList_->getIterator();
	while (it.hasNext())
		it.next()->tick(ms);

	// compile a list of all dead notifications
	LinkedList<Notification*>* deleteNotifications = new LinkedList<Notification*>();
	it = notificationList_->getIterator();
	while (it.hasNext()) {
		Notification* curr = it.next();

		if (curr->getTimeLeft() < 0)
			deleteNotifications->addFirst(curr);
	}

	// destroy all dead notifications
	while (deleteNotifications->getLength() > 0) {
		Notification* curr = deleteNotifications->poll();
		notificationList_->remove(curr);
		delete curr;
	}
}

void NotifyOverlay::addNotification(std::string str) {
	notificationList_->addFirst(new Notification(str, defaultDuration_, defaultTextSize_));
	pushOffset += defaultTextSize_;
}