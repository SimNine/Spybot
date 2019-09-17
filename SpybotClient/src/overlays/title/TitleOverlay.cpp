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

	timingUrf_ = 3000;
	timingGamelab_ = 8000;
	timingLego_ = 13000;
	timingTitle_ = 18000;
	timingSubtitle_ = 19000;

	spanFade_ = 500;
	spanHold_ = 3000;

	// first slide ("URF Productions presents")
	GUITexture* companyTexture = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, _title_urfco);
	companyTexture->setTransparency(0);
	this->addObject(companyTexture);
	companyTexture->addEffect(new GUIEffectFade(timingUrf_ - spanFade_, spanFade_, 0, 255));
	companyTexture->addEffect(new GUIEffectFade(timingUrf_ + spanHold_, spanFade_, 255, 0));

	GUITexture* companySubtitle = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, "PRESENTS", 40);
	companySubtitle->setDisplacement({ 0, companyTexture->getDimensions().y / 2 + 20 });
	companySubtitle->setTransparency(0);
	this->addObject(companySubtitle);
	companySubtitle->addEffect(new GUIEffectFade(timingUrf_ - spanFade_, spanFade_, 0, 255));
	companySubtitle->addEffect(new GUIEffectFade(timingUrf_ + spanHold_, spanFade_, 255, 0));

	// second slide ("A game originally created by GAMELAB")
	GUITexture* gamelabTexture = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, _title_gamelab);
	gamelabTexture->setTransparency(0);
	this->addObject(gamelabTexture);
	gamelabTexture->addEffect(new GUIEffectFade(timingGamelab_ - spanFade_, spanFade_, 0, 255));
	gamelabTexture->addEffect(new GUIEffectFade(timingGamelab_ + spanHold_, spanFade_, 255, 0));

	GUITexture* gamelabSubtitle = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, "A GAME ORIGINALLY CREATED BY", 40);
	gamelabSubtitle->setDisplacement({ 0, -gamelabTexture->getDimensions().y / 2 - 20 });
	gamelabSubtitle->setTransparency(0);
	this->addObject(gamelabSubtitle);
	gamelabSubtitle->addEffect(new GUIEffectFade(timingGamelab_ - spanFade_, spanFade_, 0, 255));
	gamelabSubtitle->addEffect(new GUIEffectFade(timingGamelab_ + spanHold_, spanFade_, 255, 0));

	// third slide ("for LEGO")
	GUITexture* legoTexture = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, _title_lego);
	legoTexture->setTransparency(0);
	this->addObject(legoTexture);
	legoTexture->addEffect(new GUIEffectFade(timingLego_ - spanFade_, spanFade_, 0, 255));
	legoTexture->addEffect(new GUIEffectFade(timingLego_ + spanHold_, spanFade_, 255, 0));

	GUITexture* legoSubtitle = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, "AND PRODUCED FOR", 40);
	legoSubtitle->setDisplacement({ 0, -legoTexture->getDimensions().y / 2 - 20 });
	legoSubtitle->setTransparency(0);
	this->addObject(legoSubtitle);
	legoSubtitle->addEffect(new GUIEffectFade(timingLego_ - spanFade_, spanFade_, 0, 255));
	legoSubtitle->addEffect(new GUIEffectFade(timingLego_ + spanHold_, spanFade_, 255, 0));

	// third slide ("SPYBOT: the Nightfall Incident")
	GUITexture* titleTexture = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, _title_title);
	titleTexture->setDisplacement({ 0, -titleTexture->getDimensions().y / 2 });
	titleTexture->setTransparency(0);
	this->addObject(titleTexture);
	titleTexture->addEffect(new GUIEffectFade(timingTitle_ - spanFade_*2, spanFade_*2, 0, 255));
	titleTexture->addEffect(new GUIEffectFade(timingTitle_ + spanHold_, spanFade_*2, 255, 0));

	GUITexture* subtitleTexture = new GUITexture(this, ANCHOR_CENTER, { 0, 0 }, _title_sub);
	subtitleTexture->setDisplacement({0, subtitleTexture->getDimensions().y / 2 + 10});
	subtitleTexture->setTransparency(0);
	this->addObject(subtitleTexture);
	subtitleTexture->addEffect(new GUIEffectFade(timingSubtitle_ - spanFade_*2, spanFade_*2, 0, 255));
	subtitleTexture->addEffect(new GUIEffectFade(timingSubtitle_ + spanHold_, spanFade_*2, 255, 0));

	tickCount_ = 0;
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

	if (tickCount_ > timingSubtitle_ + spanHold_ + spanFade_*2 + 500) {
		endTitles();
	}

	tickCount_ += ms;
}

bool TitleOverlay::mouseDown() {
	endTitles();
	return true;
}

void TitleOverlay::endTitles() {
	_overlayStack->removeAll();
	_overlayStack->push(_backgroundOverlay);
	_overlayStack->push(_mainOverlay);

	SDL_ShowCursor(SDL_ENABLE);
	Mix_PlayMusic(_music_title, -1);
}