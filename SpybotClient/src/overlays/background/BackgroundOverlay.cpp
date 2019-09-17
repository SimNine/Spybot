#include "Standard.h"
#include "BackgroundOverlay.h"

#include "Global.h"
#include "GUITexture.h"
#include "GlowSpeck.h"

BackgroundOverlay::BackgroundOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_clear) {
	glowList_ = new LinkedList<GlowSpeck*>();
	textBkgDisplacement_ = 0;
	mode_ = BKGMODE_STANDARD;
}

BackgroundOverlay::~BackgroundOverlay() {
	//dtor
}

void BackgroundOverlay::draw() {
	// clear the screen (black)
	switch (mode_) {
	case BKGMODE_STANDARD:
		SDL_SetRenderDrawColor(_renderer, 0, 0, 40, 0);
		break;
	case BKGMODE_RAINBOW:
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
		break;
	case BKGMODE_REDBLUE:
		SDL_SetRenderDrawColor(_renderer, 40, 0, 40, 0);
		break;
	}
	SDL_RenderClear(_renderer);

	// draw all particulates
	glowList_->forEach([] (GlowSpeck* g) { g->draw(); });

	// draw the text chunk if not in rainbow mode
	if (mode_ == BKGMODE_RAINBOW)
		return;

	SDL_Rect destRect;
	SDL_QueryTexture(_main_bkgdata, NULL, NULL, &destRect.w, &destRect.h);
	for (int x = 0; x < _screenWidth; x += destRect.w) {
		for (int y = -textBkgDisplacement_; y < _screenHeight + textBkgDisplacement_; y += destRect.h) {
			destRect.x = x;
			destRect.y = y;
			SDL_RenderCopy(_renderer, _main_bkgdata, NULL, &destRect);
		}
	}
}

void BackgroundOverlay::tick(int ms) {
	// change the text vertical displacement
	textBkgDisplacement_++;
	int txtHeight;
	SDL_QueryTexture(_main_bkgdata, NULL, NULL, NULL, &txtHeight);
	if (textBkgDisplacement_ > txtHeight) textBkgDisplacement_ -= txtHeight;

	// tick all particulates
	Iterator<GlowSpeck*> it = glowList_->getIterator();
	while (it.hasNext()) {
		it.next()->tick(ms);
	}

	// check for dead/OOB particulates
	LinkedList<GlowSpeck*> delList = LinkedList<GlowSpeck*>();
	Iterator<GlowSpeck*> it3 = glowList_->getIterator();
	while (it3.hasNext()) {
		GlowSpeck* currObj = it3.next();
		if (currObj->getTransparent()) {
			delList.addFirst(currObj);
		} else if (currObj->getPos().x + currObj->getRadius() < 0 ||
			currObj->getPos().y + currObj->getRadius() < 0 ||
			currObj->getPos().x > _screenWidth ||
			currObj->getPos().y > _screenHeight) {
			delList.addFirst(currObj);
		}
	}

	Iterator<GlowSpeck*> it2 = delList.getIterator();
	while (it2.hasNext()) {
		GlowSpeck* currObj = it2.next();
		glowList_->remove(currObj);
		delete currObj;
	}

	// the longer the delay, the more likely to add a particle
	// at 50ms, a particle is guaranteed to be added
	// decrements 50ms for each attempt until counter is below 0
	while (ms > 0) {
		if (glowList_->getLength() < 50 && rand() % 50 < ms) {
			GlowSpeck* newGlow;
			switch (mode_) {
			case BKGMODE_STANDARD:
				newGlow = new GlowSpeck({ 0, 0, 255, 0 });
				break;
			case BKGMODE_RAINBOW:
				newGlow = new GlowSpeck({ (Uint8)rand(), (Uint8)rand(), (Uint8)rand(), 0 });
				break;
			case BKGMODE_REDBLUE:
				if (rand() % 2 == 0)
					newGlow = new GlowSpeck({ 255, 0, 0, 0 });
				else
					newGlow = new GlowSpeck({ 0, 0, 255, 0 });
				break;
			default:
				newGlow = new GlowSpeck({ 255, 255, 255, 0 });
				break;
			}
			glowList_->addLast(newGlow);
		}
		ms -= 50;
	}
}

void BackgroundOverlay::setMode(BKGMODE m) {
	mode_ = m;

	// tick a few seconds in order to clear miscolored particles
	tick(3000);
}