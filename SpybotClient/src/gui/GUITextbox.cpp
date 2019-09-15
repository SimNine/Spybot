#include "Standard.h"
#include "GUITextbox.h"

#include "Global.h"
#include "ResourceLoader.h"

GUITextbox::GUITextbox(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, int maxChars, bool censored)
	: GUIContainer(anchor, disp, dims, parent, { 120, 120, 120, 140 })
{
	maxChars_ = maxChars;
	textSize_ = dims.y;
	contentText_ = "";
	censored_ = censored;
}


GUITextbox::~GUITextbox()
{

}

void GUITextbox::draw()
{
	// draw the box
	GUIContainer::drawBkg();

	// draw bounds
	if (_debug >= DEBUG_NORMAL) 
		GUIContainer::drawBounds();

	// draw text box
	if (_activeTextbox == this)
		SDL_SetRenderDrawColor(_renderer, 50, 50, 50, 200);
	else
		SDL_SetRenderDrawColor(_renderer, 100, 100, 100, 200);
	SDL_RenderFillRect(_renderer, &bounds);

	// draw current contents
	SDL_Rect tempBounds = bounds;
	tempBounds.w = 0;
	SDL_Texture* content;
	if (censored_)
	{
		std::string ast = "";
		for (int i = 0; i < contentText_.length(); i++)
			ast += "*";
		content = loadString(ast, FONT_NORMAL, textSize_, _color_white);
	}
	else
		content = loadString(contentText_, FONT_NORMAL, textSize_, _color_white);
	SDL_QueryTexture(content, NULL, NULL, &tempBounds.w, NULL);
	SDL_RenderCopy(_renderer, content, NULL, &tempBounds);
	SDL_DestroyTexture(content);

	// draw cursor
	if (_activeTextbox == this && cursorFade_ < 500)
	{
		tempBounds.x += tempBounds.w + 2;
		tempBounds.y += 5;
		tempBounds.w = 2;
		tempBounds.h -= 10;
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(_renderer, &tempBounds);
	}
}

std::string GUITextbox::getContents()
{
	return contentText_;
}

void GUITextbox::clearContents()
{
	contentText_ = "";
}

void GUITextbox::addChar(char c)
{
	if (_activeTextbox != this)
		return;

	if (c == 127) // backspace
	{
		if (contentText_.length() > 0)
			contentText_.pop_back();
	}
	else if (contentText_.length() < maxChars_)
		contentText_ += c;

	cursorFade_ = 0;
}

bool GUITextbox::mouseDown()
{
	_activeTextbox = this;
	return true;
}

bool GUITextbox::mouseUp()
{
	return true;
}

void GUITextbox::tick(int ms)
{
	cursorFade_ += ms;
	if (cursorFade_ > 1000)
		cursorFade_ -= 1000;
}