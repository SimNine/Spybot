#include "Standard.h"
#include "AchievementButton.h"

#include "Global.h"
#include "ResourceLoader.h"
#include "MainOverlay.h"

AchievementButton::AchievementButton(GUIContainer* p, ANCHOR a, Coord disp, ACHIEVEMENT achievement)
	: GUIObject(p, a, disp, { 50, 50 }) {
	achievement_ = achievement;
	mouseoverFade_ = 0;
}

AchievementButton::~AchievementButton() {
	// nada
}

void AchievementButton::setTransparency(int a) {
	currAlpha_ = a;
}

bool AchievementButton::mouseDown() {
	_mainOverlay->setSelectedAchievement(achievement_);
	return true;
}

bool AchievementButton::mouseUp() {
	return true;
}

void AchievementButton::draw() {
	if (mouseoverFade_ > 0) {
		SDL_Rect temp = bounds_;
		const int spriteDiameter = bounds_.h + (int)(0.5*bounds_.h);
		temp.x += bounds_.w / 2 - spriteDiameter/2;
		temp.y += bounds_.h / 2 - spriteDiameter/2;
		temp.w = spriteDiameter;
		temp.h = spriteDiameter;
		SDL_SetTextureAlphaMod(_main_bkgsplotch2, mouseoverFade_);
		SDL_RenderCopy(_renderer, _main_bkgsplotch2, NULL, &temp);
	}
	//SDL_RenderCopy(_renderer, _achievement_badge_large, NULL, &bounds_);
	SDL_Rect temp = bounds_;
	temp.x += bounds_.w / 2 - 25;
	temp.y += bounds_.h / 2 - 25;
	temp.w = 50;
	temp.h = 50;
	if (_progressAchievements[achievement_]) {
		SDL_SetTextureAlphaMod(_achievement_texture[achievement_], currAlpha_);
		SDL_RenderCopy(_renderer, _achievement_texture[achievement_], NULL, &temp);
	} else {
		SDL_SetTextureAlphaMod(_achievement_locked, currAlpha_);
		SDL_RenderCopy(_renderer, _achievement_locked, NULL, &temp);
	}

	if (_debug >= DEBUG_NORMAL)
		drawBounds();
}

void AchievementButton::tick(int t) {
	if (isMouseOver()) {
		mouseoverFade_ = (mouseoverFade_ + t/2 > 255) ? 255 : (mouseoverFade_ + t/2);
	} else {
		mouseoverFade_ = (mouseoverFade_ - t/2 < 0) ? 0 : (mouseoverFade_ - t/2);
	}
}

void AchievementButton::resetBounds() {
	GUIObject::recomputePosition();
}