#include "Standard.h"
#include "TitleOverlay.h"

#include "Global.h"
#include "GUITexture.h"
#include "Data.h"
#include "MainOverlay.h"
#include "BackgroundOverlay.h"
#include "GUIEffectFade.h"

TitleOverlay::TitleOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_clear) {
	int w;
	int h;

	SDL_QueryTexture(_title_company, NULL, NULL, &w, &h);
	GUITexture* companyTexture = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, { w, h }, _title_company);
	companyTexture->setTransparency(0);
	this->addObject(companyTexture);
	companyTexture->addEffect(new GUIEffectFade(500, 2500, 0, 255));
	companyTexture->addEffect(new GUIEffectFade(3000, 2500, 255, 0));

	SDL_QueryTexture(_title_title, NULL, NULL, &w, &h);
	GUITexture* titleTexture = new GUITexture(this, ANCHOR_CENTER, { 0, -h / 2 }, { w, h }, _title_title);
	titleTexture->setTransparency(0);
	this->addObject(titleTexture);
	titleTexture->addEffect(new GUIEffectFade(5500, 2500, 0, 255));
	titleTexture->addEffect(new GUIEffectFade(8000, 2500, 255, 0));

	SDL_QueryTexture(_title_subtitle, NULL, NULL, &w, &h);
	GUITexture* subtitleTexture = new GUITexture(this, ANCHOR_CENTER, { 0, h / 2 + 10 }, { w, h }, _title_subtitle);
	subtitleTexture->setTransparency(0);
	this->addObject(subtitleTexture);
	subtitleTexture->addEffect(new GUIEffectFade(6500, 2500, 0, 255));
	subtitleTexture->addEffect(new GUIEffectFade(9000, 2500, 255, 0));

	tickCount_ = 0;

	timingCompany_ = 3000;
	timingTitle_ = 8000;
	timingSubtitle_ = 9000;

	spanTiming_ = 2500;
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

	if (tickCount_ > timingSubtitle_ + spanTiming_ + 500) {
		endTitles();
	}

	tickCount_ += ms;
}

bool TitleOverlay::mouseDown() {
	advance();
	return true;
}

void TitleOverlay::advance() {
	if (tickCount_ < timingCompany_)
		tickCount_ = timingCompany_;
	else if (tickCount_ < timingTitle_)
		tickCount_ = timingTitle_;
	else
		endTitles();
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