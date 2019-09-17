#include "Standard.h"
#include "PromptBox.h"

#include "Global.h"
#include "ResourceLoader.h"
#include "GUIButton.h"
#include "Message.h"
#include "GUITextbox.h"
#include "GUIButtonParamaterized.h"

PromptBox::PromptBox(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, std::string prompt, void(*onOk) (PromptBox* thisObj), void(*onCancel) (PromptBox* thisObj))
	: GUIContainer(parent, anchor, disp, dims, _color_bkg_standard) {
	promptText_ = prompt;

	okButton_ = new GUIButtonParamaterized<PromptBox*>(this, ANCHOR_SOUTHWEST, { 20, -20 }, "OK", 50, onOk, this);
	this->addObject(okButton_);

	cancelButton_ = new GUIButtonParamaterized<PromptBox*>(this, ANCHOR_SOUTHEAST, { -20, -20 }, "CANCEL", 50, onCancel, this);
	this->addObject(cancelButton_);

	entryField_ = new GUITextbox(this, ANCHOR_NORTHWEST, { 5, 50 }, { bounds_.w - 10, textSize_ }, DEFAULT_MSG_TEXTSIZE, false);
	this->addObject(entryField_);
}

PromptBox::~PromptBox() {
	// dtor
}

void PromptBox::draw() {
	// draw the box
	GUIContainer::drawBkg();

	// draw the contents (only buttons in this case)
	GUIContainer::drawContents();

	// draw bounds
	if (_debug >= DEBUG_NORMAL)
		GUIContainer::drawBounds();

	// draw prompt
	SDL_Rect tempBounds;
	SDL_Texture* name = loadString(promptText_, FONT_NORMAL, textSize_, { 255, 255, 255, (Uint8)currAlpha_ });
	SDL_QueryTexture(name, NULL, NULL, &tempBounds.w, &tempBounds.h);
	tempBounds.x = bounds_.x + 5;
	tempBounds.y = bounds_.y + 5;
	SDL_RenderCopy(_renderer, name, NULL, &tempBounds);
	SDL_DestroyTexture(name);
}

std::string PromptBox::getContents() {
	return entryField_->getContents();
}

void PromptBox::clearContents() {
	entryField_->clearContents();
}

void PromptBox::addChar(char c) {
	if (c == 13) { // enter/return
		okButton_->mouseDown();
		okButton_->mouseUp();
	} else if (c == 27) { // escape
		cancelButton_->mouseDown();
		cancelButton_->mouseUp();
	} else
		entryField_->addChar(c);
}

GUITextbox* PromptBox::getTextbox() {
	return entryField_;
}