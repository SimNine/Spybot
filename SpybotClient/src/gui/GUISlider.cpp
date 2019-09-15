#include "Standard.h"
#include "GUISlider.h"

#include "Global.h"
#include "GUIContainer.h"

GUISlider::GUISlider(ANCHOR a, Coord disp, Coord dims, GUIContainer* p, void(*func) (float))
	: GUIObject(a, disp, dims, p) {
	sliderBounds.x = bounds.x;
	sliderBounds.y = bounds.y;
	sliderBounds.w = 20;
	sliderBounds.h = bounds.h;
	this->func = func;
	sliderVal = sliderBounds.x - bounds.x;
}

GUISlider::~GUISlider() {
	//dtor
}

void GUISlider::setTransparency(int a) {
	if (a > 255) currAlpha = 255;
	else if (a < 0) currAlpha = 0;
	else currAlpha = a;
}

bool GUISlider::mouseDown() {
	pressed = true;
	return true;
}

bool GUISlider::mouseUp() {
	pressed = false;
	return true;
}

void GUISlider::draw() {
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, currAlpha);
	SDL_RenderDrawLine(_renderer,
		bounds.x + sliderBounds.w / 2,
		bounds.y + bounds.h / 2,
		bounds.x + bounds.w - sliderBounds.w / 2,
		bounds.y + bounds.h / 2);
	SDL_RenderDrawRect(_renderer, &sliderBounds);

	if (_debug >= DEBUG_NORMAL) drawBounds();
}

void GUISlider::tick(int ms) {
	fadeStep(ms);

	if (pressed) {
		if (_mousePos.x < bounds.x + sliderBounds.w / 2) sliderBounds.x = bounds.x;
		else if (_mousePos.x > bounds.x + bounds.w - sliderBounds.w / 2) sliderBounds.x = bounds.x + bounds.w - sliderBounds.w;
		else sliderBounds.x = _mousePos.x - sliderBounds.w / 2;

		sliderVal = sliderBounds.x - bounds.x;
		float dinger = (float)sliderVal / bounds.w;
		func(dinger);
	}
}

void GUISlider::resetBounds() {
	recomputePosition();
	sliderBounds.x = bounds.x + sliderVal - sliderBounds.w / 2;
	sliderBounds.y = bounds.y;
}
