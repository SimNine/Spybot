#include "Standard.h"
#include "GUITextbox.h"

#include "Global.h"
#include "ResourceLoader.h"

GUITextbox::GUITextbox(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, int maxChars, bool censored)
	: GUIObject(parent, anchor, disp, dims) {
	maxChars_ = maxChars;
	textSize_ = dims.y;
	contentText_ = "";
	censored_ = censored;
}


GUITextbox::~GUITextbox() {
	// dtor
}

void GUITextbox::draw() {
	// draw the box
	int newAlpha = (int)((currAlpha_ / 255.0)*140);
	SDL_SetRenderDrawColor(_renderer, 120, 120, 120, newAlpha);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(_renderer, &bounds_);

	// draw bounds
	if (_debug >= DEBUG_NORMAL)
		GUIObject::drawBounds();

	// draw text box
	if (_activeTextbox == this) {
		int newAlpha = (int)((currAlpha_ / 255.0)*200);
		SDL_SetRenderDrawColor(_renderer, 50, 50, 50, newAlpha);
	} else {
		int newAlpha = (int)((currAlpha_ / 255.0)*200);
		SDL_SetRenderDrawColor(_renderer, 100, 100, 100, newAlpha);
	}
	SDL_RenderFillRect(_renderer, &bounds_);

	// draw current contents
	SDL_Rect tempBounds = bounds_;
	tempBounds.w = 0;
	SDL_Texture* content;
	if (censored_) {
		std::string ast = "";
		for (unsigned int i = 0; i < contentText_.length(); i++)
			ast += "*";
		content = loadString(ast, FONT_NORMAL, textSize_, { 255, 255, 255, (Uint8)currAlpha_ });
	} else
		content = loadString(contentText_, FONT_NORMAL, textSize_, { 255, 255, 255, (Uint8)currAlpha_ });
	SDL_QueryTexture(content, NULL, NULL, &tempBounds.w, NULL);
	SDL_RenderCopy(_renderer, content, NULL, &tempBounds);
	SDL_DestroyTexture(content);

	// draw cursor
	if (_activeTextbox == this && cursorFade_ < 500) {
		tempBounds.x += tempBounds.w + 2;
		tempBounds.y += 5;
		tempBounds.w = 2;
		tempBounds.h -= 10;
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, currAlpha_);
		SDL_RenderFillRect(_renderer, &tempBounds);
	}
}

std::string GUITextbox::getContents() {
	return contentText_;
}

void GUITextbox::clearContents() {
	contentText_ = "";
}

void GUITextbox::addChar(char c) {
	if (_activeTextbox != this)
		return;

	if (c == 127) { // backspace
		if (contentText_.length() > 0)
			contentText_.pop_back();
	} else if (contentText_.length() < maxChars_)
		contentText_ += c;

	cursorFade_ = 0;
}

bool GUITextbox::mouseDown() {
	_activeTextbox = this;
	return true;
}

bool GUITextbox::mouseUp() {
	return true;
}

void GUITextbox::tick(int ms) {
	processEffects(ms);
	cursorFade_ += ms;
	if (cursorFade_ > 1000)
		cursorFade_ -= 1000;
}

void GUITextbox::setTransparency(int alpha) {
	if (alpha > 255)
		currAlpha_ = 255;
	else if (alpha < 0)
		currAlpha_ = 0;
	else
		currAlpha_ = alpha;
}

void GUITextbox::resetBounds() {
	recomputePosition();

	if (parent_ == NULL) {
		setDisplacement({ 0,0 });
		setDimensions({ _screenWidth, _screenHeight });
	}
}