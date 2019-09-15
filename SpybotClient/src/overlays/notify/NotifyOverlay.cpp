#include "Standard.h"
#include "NotifyOverlay.h"

#include "Global.h"
#include "GUITexture.h"
#include "ResourceLoader.h"
#include "PopupNotification.h"
#include "PopupAchievement.h"

NotifyOverlay::NotifyOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_clear) {
	notificationList_ = new LinkedList<PopupNotification*>();
	achievementList_ = new LinkedList<PopupAchievement*>();
	pushOffset_ = 0;
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
	Iterator<PopupNotification*> it = notificationList_->getIterator();
	while (it.hasNext()) {
		PopupNotification* curr = it.next();

		// create texture, set bounds
		currStr = loadString(curr->getContent(), FONT_BOLD, curr->getSize(), { 255, 255, 255, 255 });
		SDL_QueryTexture(currStr, NULL, NULL, &strBounds.w, &strBounds.h);
		yOffset -= strBounds.h;
		strBounds.x = _screenWidth - strBounds.w - 5;
		strBounds.y = _screenHeight + yOffset + pushOffset_;

		// set fade
		currTime = curr->getTimeLeft();
		if (currTime < 1000)
			SDL_SetTextureAlphaMod(currStr, currTime / 4);

		// render and destroy
		SDL_RenderCopy(_renderer, currStr, NULL, &strBounds);
		SDL_DestroyTexture(currStr);
	}

	// draw the topmost achievement (if there is one)
	if (achievementList_->getLength() > 0) {
		PopupAchievement* popup = achievementList_->getFirst();
		ACHIEVEMENT a = popup->getAchievement();

		// create a texture for the achievement string
		SDL_Texture* achievementTitle = loadString(_achievement_name[a], FONT_NORMAL, 30, _color_white);
		int wTitle, hTitle;
		SDL_QueryTexture(achievementTitle, NULL, NULL, &wTitle, &hTitle);

		// create the popup rectangle
		SDL_Rect popupBounds;
		popupBounds.w = wTitle + 30 + 50; // 30 for padding, 50 for achievement icon
		popupBounds.h = 70;
		popupBounds.x = (_screenWidth / 2) - (popupBounds.w / 2);
		popupBounds.y = 10;

		// calculate the transparency
		Uint8 achievementAlpha;
		if (popup->getTimeLeft() > 4000) {
			achievementAlpha = (1000 - (popup->getTimeLeft() - 4000)) / 4;
		} else if (popup->getTimeLeft() < 1000) {
			achievementAlpha = (popup->getTimeLeft() / 4);
		} else {
			achievementAlpha = 255;
		}

		// draw the rectangle background
		Uint8 newAlpha = (Uint8)(_color_bkg_standard.a * (achievementAlpha / 255.0));
		SDL_SetRenderDrawColor(_renderer, _color_bkg_standard.r, _color_bkg_standard.g, _color_bkg_standard.b, newAlpha);
		SDL_RenderFillRect(_renderer, &popupBounds);

		// draw the icon
		SDL_Rect achievementIconBounds;
		achievementIconBounds.w = 50;
		achievementIconBounds.h = 50;
		achievementIconBounds.x = popupBounds.x + 10;
		achievementIconBounds.y = popupBounds.y + 10;
		SDL_SetTextureAlphaMod(_achievement_texture[popup->getAchievement()], achievementAlpha);
		SDL_RenderCopy(_renderer, _achievement_texture[popup->getAchievement()], NULL, &achievementIconBounds);

		// draw the string
		SDL_Rect achievementTextBounds;
		achievementTextBounds.w = wTitle;
		achievementTextBounds.h = hTitle;
		achievementTextBounds.x = popupBounds.x + 70;
		achievementTextBounds.y = popupBounds.y + 15;
		SDL_SetTextureAlphaMod(achievementTitle, achievementAlpha);
		SDL_RenderCopy(_renderer, achievementTitle, NULL, &achievementTextBounds);
	}
}

void NotifyOverlay::tick(int ms) {
	// update the push offset
	if (pushOffset_ > 0)
		pushOffset_ -= (ms / 20 + 1);
	if (pushOffset_ < 0)
		pushOffset_ = 0;

	// update the time for each notification
	Iterator<PopupNotification*> it = notificationList_->getIterator();
	while (it.hasNext())
		it.next()->tick(ms);

	// compile a list of all dead notifications
	LinkedList<PopupNotification*>* deleteNotifications = new LinkedList<PopupNotification*>();
	it = notificationList_->getIterator();
	while (it.hasNext()) {
		PopupNotification* curr = it.next();

		if (curr->getTimeLeft() < 0)
			deleteNotifications->addFirst(curr);
	}

	// destroy all dead notifications
	while (deleteNotifications->getLength() > 0) {
		PopupNotification* curr = deleteNotifications->poll();
		notificationList_->remove(curr);
		delete curr;
	}

	// update the time for the top achievement, check if it is dead, remove it if it is
	if (achievementList_->getLength() > 0) {
		achievementList_->getFirst()->tick(ms);
		if (achievementList_->getFirst()->getTimeLeft() <= 0) {
			delete achievementList_->poll();
		}
	}
}

void NotifyOverlay::addNotification(std::string str) {
	notificationList_->addFirst(new PopupNotification(str, defaultDuration_, defaultTextSize_));
	pushOffset_ += defaultTextSize_;
}

void NotifyOverlay::addAchievement(ACHIEVEMENT a) {
	achievementList_->addFirst(new PopupAchievement(a, 5000));
}