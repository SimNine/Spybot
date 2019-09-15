#include "Standard.h"
#include "GUITexture.h"

#include "ResourceLoader.h"
#include "Global.h"

GUITexture::GUITexture(ANCHOR a, Coord disp, SDL_Texture* tex, Coord dims, GUIContainer* par)
	: GUIObject(a, disp, dims, par) {
	texture_ = tex;
	willDestroyTexture_ = false;
}

GUITexture::GUITexture(ANCHOR a, Coord disp, SDL_Texture* tex, Coord dims, bool destroyTex, GUIContainer* par)
	: GUIObject(a, disp, dims, par) {
	texture_ = tex;
	willDestroyTexture_ = destroyTex;
}

GUITexture::GUITexture(ANCHOR a, Coord disp, std::string tex, Coord dims, GUIContainer* par)
	: GUIObject(a, disp, dims, par) {
	texture_ = loadTexture(tex);
	willDestroyTexture_ = true;
}

GUITexture::GUITexture(ANCHOR a, Coord disp, std::string str, int fontSize, GUIContainer* parent)
	: GUIObject(a, disp, { 0, 0 }, parent) {
	texture_ = loadString(str, FONT_NORMAL, fontSize, _color_white);
	int w, h;
	SDL_QueryTexture(texture_, NULL, NULL, &w, &h);
	setBounds(disp, { w, h });
	willDestroyTexture_ = true;
}

GUITexture::~GUITexture() {
	if (willDestroyTexture_)
		SDL_DestroyTexture(texture_);
}

bool GUITexture::mouseDown() {
	return false;
}

bool GUITexture::mouseUp() {
	return true;
}

void GUITexture::draw() {
	SDL_RenderCopy(_renderer, texture_, NULL, &bounds);

	if (_debug >= DEBUG_NORMAL) drawBounds();
}

void GUITexture::setTransparency(int a) {
	if (a > 255) currAlpha = 255;
	else if (a < 0) currAlpha = 0;
	else currAlpha = a;

	SDL_SetTextureAlphaMod(texture_, currAlpha);
}

void GUITexture::tick(int ms) {
	fadeStep(ms);
	return;
}

SDL_Texture* GUITexture::swapTexture(SDL_Texture* t) {
	SDL_Texture* temp = texture_;
	texture_ = t;
	return temp;
}

void GUITexture::resetBounds() {
	recomputePosition();
}
