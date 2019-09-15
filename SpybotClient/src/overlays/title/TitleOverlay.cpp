#include "Standard.h"
#include "TitleOverlay.h"

#include "Global.h"
#include "GUITexture.h"
#include "Data.h"
#include "MainOverlay.h"
#include "BackgroundOverlay.h"

TitleOverlay::TitleOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _SCREEN_WIDTH, _SCREEN_HEIGHT }, NULL) {
	int w;
	int h;
	SDL_QueryTexture(_title_company, NULL, NULL, &w, &h);
	addObject(new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, { w, h }, _title_company));
	SDL_QueryTexture(_title_title, NULL, NULL, &w, &h);
	addObject(new GUITexture(this, ANCHOR_CENTER, { 0, -h / 2 }, { w, h }, _title_title));
	SDL_QueryTexture(_title_subtitle, NULL, NULL, &w, &h);
	addObject(new GUITexture(this, ANCHOR_CENTER, { 0, h / 2 + 10 }, { w, h }, _title_subtitle));

	//sprites = new LinkedList<GUITexture*>();

	tickCount = 0;

	timingCompany = 3000;
	timingTitle = 8000;
	timingSubtitle = 9000;

	spanTiming = 2500;
}

TitleOverlay::~TitleOverlay() {
	//dtor
}

void TitleOverlay::draw() {
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);

	drawContents();
}

void TitleOverlay::tick(int ms) {
	// tick all GUIObjects
	GUIContainer::tick(ms);

	if (tickCount <= timingCompany - spanTiming) {
		SDL_SetTextureAlphaMod(_title_company, 0);
	} else if (tickCount > timingCompany - spanTiming && tickCount <= timingCompany) {
		SDL_SetTextureAlphaMod(_title_company, (int)(((double)(tickCount - timingCompany + spanTiming) / spanTiming)*255.0));
	} else if (tickCount > timingCompany && tickCount <= timingCompany + spanTiming) {
		SDL_SetTextureAlphaMod(_title_company, (int)(((double)(timingCompany - tickCount + spanTiming) / spanTiming)*255.0));
	} else if (tickCount > timingCompany + spanTiming) {
		SDL_SetTextureAlphaMod(_title_company, 0);
	}

	if (tickCount <= timingTitle - spanTiming) {
		SDL_SetTextureAlphaMod(_title_title, 0);
	} else if (tickCount > timingTitle - spanTiming && tickCount <= timingTitle) {
		SDL_SetTextureAlphaMod(_title_title, (int)(((double)(tickCount - timingTitle + spanTiming) / spanTiming)*255.0));
	} else if (tickCount > timingTitle && tickCount <= timingTitle + spanTiming) {
		SDL_SetTextureAlphaMod(_title_title, (int)(((double)(timingTitle - tickCount + spanTiming) / spanTiming)*255.0));
	} else if (tickCount > timingTitle + spanTiming) {
		SDL_SetTextureAlphaMod(_title_title, 0);
	}

	if (tickCount <= timingSubtitle - spanTiming) // before fade in
	{
		SDL_SetTextureAlphaMod(_title_subtitle, 0);
	} else if (tickCount > timingSubtitle - spanTiming && tickCount <= timingSubtitle) // fading in
	{
		SDL_SetTextureAlphaMod(_title_subtitle, (int)(((double)(tickCount - timingSubtitle + spanTiming) / spanTiming)*255.0));
	} else if (tickCount > timingSubtitle && tickCount <= timingSubtitle + spanTiming) // fading out
	{
		SDL_SetTextureAlphaMod(_title_subtitle, (int)(((double)(timingSubtitle - tickCount + spanTiming) / spanTiming)*255.0));
	} else if (tickCount > timingSubtitle + spanTiming) {
		SDL_SetTextureAlphaMod(_title_subtitle, 0);
	}

	if (tickCount > timingSubtitle + spanTiming + 500) {
		endTitles();
	}

	tickCount += ms;
}

bool TitleOverlay::mouseDown() {
	advance();
	return true;
}

void TitleOverlay::advance() {
	if (tickCount < timingCompany) tickCount = timingCompany;
	else if (tickCount < timingTitle) tickCount = timingTitle;
	else endTitles();
}

void TitleOverlay::endTitles() {
	_overlayStack->removeAll();
	_overlayStack->push(_backgroundOverlay);
	_overlayStack->push(_mainOverlay);

	SDL_SetTextureAlphaMod(_title_subtitle, 255);
	SDL_SetTextureAlphaMod(_title_title, 255);
	SDL_ShowCursor(SDL_ENABLE);
	Mix_PlayMusic(_music_title, -1);
}
