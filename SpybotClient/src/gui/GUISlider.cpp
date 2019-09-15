#include "Standard.h"
#include "GUISlider.h"

#include "Global.h"
#include "GUIContainer.h"

GUISlider::GUISlider(GUIContainer* p, ANCHOR a, Coord disp, Coord dims, void(*func) (float), float initValue)
	: GUIObject(p, a, disp, dims) {
	sliderBounds_.x = bounds_.x;
	sliderBounds_.y = bounds_.y;
	sliderBounds_.w = 20;
	sliderBounds_.h = bounds_.h;
	this->func = func;
	sliderVal_ = (int)((double)bounds_.w * initValue);
}

GUISlider::~GUISlider() {
	//dtor
}

void GUISlider::setTransparency(int a) {
	if (a > 255)
		currAlpha_ = 255;
	else if (a < 0)
		currAlpha_ = 0;
	else
		currAlpha_ = a;
}

bool GUISlider::mouseDown() {
	pressed_ = true;
	return true;
}

bool GUISlider::mouseUp() {
	pressed_ = false;
	return true;
}

void GUISlider::draw() {
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, currAlpha_);
	SDL_RenderDrawLine(_renderer,
		bounds_.x + sliderBounds_.w / 2,
		bounds_.y + bounds_.h / 2,
		bounds_.x + bounds_.w - sliderBounds_.w / 2,
		bounds_.y + bounds_.h / 2);
	SDL_RenderDrawRect(_renderer, &sliderBounds_);

	if (_debug >= DEBUG_NORMAL)
		drawBounds();
}

void GUISlider::tick(int ms) {
	processEffects(ms);

	if (pressed_) {
		if (_mousePos.x < bounds_.x + sliderBounds_.w / 2)
			sliderBounds_.x = bounds_.x;
		else if (_mousePos.x > bounds_.x + bounds_.w - sliderBounds_.w / 2)
			sliderBounds_.x = bounds_.x + bounds_.w - sliderBounds_.w;
		else
			sliderBounds_.x = _mousePos.x - sliderBounds_.w / 2;

		sliderVal_ = sliderBounds_.x - bounds_.x;
		float dinger = (float)sliderVal_ / bounds_.w;
		func(dinger);
	}
}

void GUISlider::resetBounds() {
	recomputePosition();
	sliderBounds_.x = bounds_.x + sliderVal_ - sliderBounds_.w / 2;
	sliderBounds_.y = bounds_.y;
}