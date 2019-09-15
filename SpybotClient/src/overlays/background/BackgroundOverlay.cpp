#include "Standard.h"
#include "BackgroundOverlay.h"

#include "Global.h"
#include "GUITexture.h"
#include "GlowSpeck.h"

BackgroundOverlay::BackgroundOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _SCREEN_WIDTH, _SCREEN_HEIGHT }, NULL) {
	glowList_ = new LinkedList<GlowSpeck*>();
	textBkgDisplacement_ = 0;
}

BackgroundOverlay::~BackgroundOverlay() {
	//dtor
}

void BackgroundOverlay::draw() {
	// clear the screen (black)
	SDL_SetRenderDrawColor(_renderer, 0, 0, 40, 0);
	SDL_RenderClear(_renderer);

	// draw all particulates
	glowList_->forEach([] (GlowSpeck* g) { g->draw(); });

	// draw the text chunk
	SDL_Rect destRect;
	SDL_QueryTexture(_main_bkgdata, NULL, NULL, &destRect.w, &destRect.h);
	for (int x = 0; x < _SCREEN_WIDTH; x += destRect.w) {
		for (int y = -textBkgDisplacement_; y < _SCREEN_HEIGHT + textBkgDisplacement_; y += destRect.h) {
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
			currObj->getPos().x > _SCREEN_WIDTH ||
			currObj->getPos().y > _SCREEN_HEIGHT) {
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
	if (glowList_->getLength() < 50 && rand() % 50 < ms) {
		GlowSpeck* newGlow = new GlowSpeck();
		glowList_->addLast(newGlow);
	}
}