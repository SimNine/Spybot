#include "Standard.h"
#include "GUIButton.h"

#include "ResourceLoader.h"
#include "Global.h"

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, std::string str, int fontSize, void(*func) (void))
	: GUIObject(parent, a, disp, { 0, 0 }) {
	this->func = func;
	this->bkgNormal_ = loadString(str, FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
	this->bkgOver_ = loadString(str, FONT_NORMAL, fontSize, { 150, 150, 150, 255 });
	this->bkgPressed_ = loadString(str, FONT_NORMAL, fontSize, { 75, 75, 75, 150 });;

	int w, h;
	SDL_QueryTexture(bkgNormal_, NULL, NULL, &w, &h);
	setDimensions({ w, h });
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims, void(*func) (void),
	std::string bkgN)
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal_ = loadTexture(bkgN);
	this->bkgOver_ = loadTexture(bkgN);
	this->bkgPressed_ = loadTexture(bkgN);
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims,
	void(*func) (void), std::string bkgN, std::string bkgO)
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal_ = loadTexture(bkgN);
	this->bkgOver_ = loadTexture(bkgO);
	this->bkgPressed_ = loadTexture(bkgO);
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims,
	void(*func) (void), std::string bkgN, std::string bkgO, std::string bkgP)
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal_ = loadTexture(bkgN);
	this->bkgOver_ = loadTexture(bkgO);
	this->bkgPressed_ = loadTexture(bkgP);
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims,
	void(*func) (void))
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal_ = NULL;
	this->bkgOver_ = NULL;
	this->bkgPressed_ = NULL;
}

GUIButton::~GUIButton() {
	if (bkgNormal_ != NULL)
		SDL_DestroyTexture(bkgNormal_);
	if (bkgOver_ != NULL)
		SDL_DestroyTexture(bkgOver_);
	if (bkgPressed_ != NULL)
		SDL_DestroyTexture(bkgPressed_);
}

void GUIButton::draw() {
	if (_mousePressed && isMouseOver() && bkgPressed_ != NULL) {
		SDL_RenderCopy(_renderer, bkgPressed_, NULL, &bounds_);
	} else if (!_mousePressed && isMouseOver() && bkgOver_ != NULL) {
		SDL_RenderCopy(_renderer, bkgOver_, NULL, &bounds_);
	} else if (bkgNormal_ != NULL) {
		SDL_RenderCopy(_renderer, bkgNormal_, NULL, &bounds_);
	}

	if (_debug >= DEBUG_NORMAL)
		drawBounds();
}

bool GUIButton::mouseDown() {
	pressed_ = true;
	return true;
}

bool GUIButton::mouseUp() {
	if (pressed_)
		func();
	pressed_ = false;
	return true;
}

void GUIButton::setTransparency(int a) {
	if (a > 255)
		currAlpha_ = 255;
	else if (a < 0)
		currAlpha_ = 0;
	else
		currAlpha_ = a;

	SDL_SetTextureAlphaMod(bkgNormal_, currAlpha_);
	SDL_SetTextureAlphaMod(bkgOver_, currAlpha_);
	SDL_SetTextureAlphaMod(bkgPressed_, currAlpha_);
}

void GUIButton::tick(int ms) {
	processEffects(ms);
	return;
}

void GUIButton::resetBounds() {
	recomputePosition();
}
