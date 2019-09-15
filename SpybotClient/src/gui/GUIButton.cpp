#include "Standard.h"
#include "GUIButton.h"

#include "ResourceLoader.h"
#include "Global.h"

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, std::string str, int fontSize, void(*func) (void))
	: GUIObject(parent, a, disp, { 0, 0 }) {
	this->func = func;
	this->bkgNormal = loadString(str, FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
	this->bkgOver = loadString(str, FONT_NORMAL, fontSize, { 150, 150, 150, 255 });
	this->bkgPressed = loadString(str, FONT_NORMAL, fontSize, { 150, 150, 150, 150 });;

	int w, h;
	SDL_QueryTexture(bkgNormal, NULL, NULL, &w, &h);
	setBounds(disp, { w, h });
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims, void(*func) (void),
	std::string bkgN)
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal = loadTexture(bkgN);
	this->bkgOver = loadTexture(bkgN);
	this->bkgPressed = loadTexture(bkgN);
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims,
	void(*func) (void), std::string bkgN, std::string bkgO)
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal = loadTexture(bkgN);
	this->bkgOver = loadTexture(bkgO);
	this->bkgPressed = loadTexture(bkgO);
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims,
	void(*func) (void), std::string bkgN, std::string bkgO, std::string bkgP)
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal = loadTexture(bkgN);
	this->bkgOver = loadTexture(bkgO);
	this->bkgPressed = loadTexture(bkgP);
}

GUIButton::GUIButton(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims,
	void(*func) (void))
	: GUIObject(parent, a, disp, dims) {
	this->func = func;
	this->bkgNormal = NULL;
	this->bkgOver = NULL;
	this->bkgPressed = NULL;
}

GUIButton::~GUIButton() {
	if (bkgNormal != NULL) SDL_DestroyTexture(bkgNormal);
	if (bkgOver != NULL) SDL_DestroyTexture(bkgOver);
	if (bkgPressed != NULL) SDL_DestroyTexture(bkgPressed);
}

void GUIButton::draw() {
	if (_mousePressed && isMouseOver() && bkgPressed != NULL) {
		SDL_RenderCopy(_renderer, bkgPressed, NULL, &bounds);
	} else if (!_mousePressed && isMouseOver() && bkgOver != NULL) {
		SDL_RenderCopy(_renderer, bkgOver, NULL, &bounds);
	} else if (bkgNormal != NULL) {
		SDL_RenderCopy(_renderer, bkgNormal, NULL, &bounds);
	}

	if (_debug >= DEBUG_NORMAL) drawBounds();
}

bool GUIButton::mouseDown() {
	pressed = true;
	return true;
}

bool GUIButton::mouseUp() {
	if (pressed) func();
	pressed = false;
	return true;
}

void GUIButton::setTransparency(int a) {
	if (a > 255) currAlpha = 255;
	else if (a < 0) currAlpha = 0;
	else currAlpha = a;

	SDL_SetTextureAlphaMod(bkgNormal, currAlpha);
	SDL_SetTextureAlphaMod(bkgOver, currAlpha);
	SDL_SetTextureAlphaMod(bkgPressed, currAlpha);
}

void GUIButton::tick(int ms) {
	fadeStep(ms);
	return;
}

void GUIButton::resetBounds() {
	recomputePosition();
}
