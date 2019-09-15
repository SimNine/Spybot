#include "Standard.h"
#include "TextEntryBox.h"

#include "Global.h"
#include "ResourceLoader.h"
#include "GUIButton.h"
#include "Message.h"

TextEntryBox::TextEntryBox(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, std::string prompt, void(*onOk) (void), void(*onCancel) (void))
	: GUIContainer(anchor, disp, dims, parent, { 120, 120, 120, 140 })
{
	promptText_ = prompt;

	okButton_ = new GUIButton(ANCHOR_SOUTHWEST, { 20, -80 }, "Ok", this, onOk);
	addObject(okButton_);

	cancelButton_ = new GUIButton(ANCHOR_SOUTHEAST, { -120, - 80 }, "Cancel", this, onCancel);
	addObject(cancelButton_);
}


TextEntryBox::~TextEntryBox()
{

}

void TextEntryBox::draw()
{
	// draw the box
	GUIContainer::drawBkg();

	// draw the contents (only buttons in this case)
	GUIContainer::drawContents();

	// draw bounds
	if (debug >= DEBUG_NORMAL) 
		GUIContainer::drawBounds();

	// draw prompt
	SDL_Rect tempBounds;
	SDL_Texture* name = loadString(promptText_, FONT_NORMAL, textSize_, { 255, 255, 255, 255 });
	SDL_QueryTexture(name, NULL, NULL, &tempBounds.w, &tempBounds.h);
	tempBounds.x = bounds.x + 5;
	tempBounds.y = bounds.y + 5;
	SDL_RenderCopy(gRenderer, name, NULL, &tempBounds);
	SDL_DestroyTexture(name);

	// draw text box
	tempBounds.x = bounds.x + 5;
	tempBounds.y = bounds.y + 5 + tempBounds.h + 5;
	tempBounds.w = bounds.w - 10;
	tempBounds.h = textSize_;
	SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 200);
	SDL_RenderFillRect(gRenderer, &tempBounds);

	// draw current contents
	SDL_Texture* content = loadString(contentText_, FONT_NORMAL, textSize_, { 255, 255, 255, 255 });
	SDL_QueryTexture(content, NULL, NULL, &tempBounds.w, NULL);
	SDL_RenderCopy(gRenderer, content, NULL, &tempBounds);
	SDL_DestroyTexture(content);
}

std::string TextEntryBox::getContents()
{
	return contentText_;
}

void TextEntryBox::clearContents()
{
	contentText_ = "";
}

void TextEntryBox::addChar(char c)
{
	if (c == 127) // backspace
	{
		if (contentText_.length() > 0)
			contentText_.pop_back();
	}
	else if (c == 13) // enter/return
	{
		okButton_->mouseDown();
		okButton_->mouseUp();
	}
	else if (c == 27) // escape
	{
		cancelButton_->mouseDown();
		cancelButton_->mouseUp();
	}
	else if (contentText_.length() < DEFAULT_MSG_TEXTSIZE)
		contentText_ += c;
}