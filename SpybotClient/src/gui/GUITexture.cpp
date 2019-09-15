#include "Standard.h"
#include "GUITexture.h"

#include "ResourceLoader.h"
#include "Global.h"

GUITexture::GUITexture(GUIContainer* par, ANCHOR a, Coord disp, Coord dims, SDL_Texture* tex)
	: GUIObject(par, a, disp, dims) {
	texture_ = tex;
	willDestroyTexture_ = false;
}

GUITexture::GUITexture(GUIContainer* par, ANCHOR a, Coord disp, Coord dims, SDL_Texture* tex, bool destroyTex)
	: GUIObject(par, a, disp, dims) {
	texture_ = tex;
	willDestroyTexture_ = destroyTex;
}

GUITexture::GUITexture(GUIContainer* par, ANCHOR a, Coord disp, Coord dims, std::string tex)
	: GUIObject(par, a, disp, dims) {
	texture_ = loadTexture(tex);
	willDestroyTexture_ = true;
}

GUITexture::GUITexture(GUIContainer* par, ANCHOR a, Coord disp, std::string tex)
	: GUIObject(par, a, disp, { 0,0 }) {
	texture_ = loadTexture(tex);
	int w, h;
	SDL_QueryTexture(texture_, NULL, NULL, &w, &h);
	setDimensions({ w, h });
	willDestroyTexture_ = true;
}

GUITexture::GUITexture(GUIContainer* parent, ANCHOR a, Coord disp, std::string str, int fontSize)
	: GUIObject(parent, a, disp, { 0, 0 }) {
	texture_ = loadString(str, FONT_NORMAL, fontSize, _color_white);
	int w, h;
	SDL_QueryTexture(texture_, NULL, NULL, &w, &h);
	setDimensions({ w, h });
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
	SDL_SetTextureAlphaMod(texture_, currAlpha_);
	SDL_RenderCopy(_renderer, texture_, NULL, &bounds_);

	if (_debug >= DEBUG_NORMAL)
		drawBounds();
}

void GUITexture::setTransparency(int a) {
	if (a > 255)
		currAlpha_ = 255;
	else if (a < 0)
		currAlpha_ = 0;
	else
		currAlpha_ = a;
}

void GUITexture::tick(int ms) {
	processEffects(ms);
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
