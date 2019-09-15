#include "Standard.h"
#include "ChatDisplay.h"

#include "Global.h"
#include "Message.h"
#include "ResourceLoader.h"
#include "Client.h"
#include "ConnectionManager.h"

ChatDisplay::ChatDisplay(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent, int logSize)
	: GUIContainer(anchor, disp, dims, parent, { 0, 0, 0, 100 }) {
	active_ = false;
	logSize_ = logSize;
	chatLog_ = new LinkedList<std::string*>();
	chatLogTiming_ = new LinkedList<int*>();
}


ChatDisplay::~ChatDisplay() {
	// dtor
}

void ChatDisplay::draw() {
	if (active_) { // chat window is active
		// draw background rectangle
		GUIContainer::drawBkg();

		// set up constants
		SDL_Texture* currStr = NULL;
		int yOffset = -5;
		SDL_Rect strBounds;

		// draw prompt
		currStr = loadString("> " + inputText_, FONT_NORMAL, textSize_, { 255, 255, 255, 255 });
		SDL_QueryTexture(currStr, NULL, NULL, &strBounds.w, &strBounds.h);
		yOffset -= strBounds.h;
		strBounds.x = bounds.x + 5;
		strBounds.y = bounds.y + bounds.h + yOffset;
		SDL_RenderCopy(_renderer, currStr, NULL, &strBounds);
		SDL_DestroyTexture(currStr);

		// for each notification
		for (int i = 0; i < chatLog_->getLength(); i++) {
			// create texture, set bounds
			currStr = loadString(*chatLog_->getObjectAt(i), FONT_NORMAL, textSize_, { 255, 255, 255, 255 });
			SDL_QueryTexture(currStr, NULL, NULL, &strBounds.w, &strBounds.h);
			yOffset -= strBounds.h;
			strBounds.x = bounds.x + 5;
			strBounds.y = bounds.y + bounds.h + yOffset;

			// render and destroy
			SDL_RenderCopy(_renderer, currStr, NULL, &strBounds);
			SDL_DestroyTexture(currStr);
		}
	} else // chat window is not active
	{
		// set up constants
		int currTime = -1;
		SDL_Texture* currStr = NULL;
		int yOffset = -5;
		SDL_Rect strBounds;

		// draw prompt
		currStr = loadString("> " + inputText_, FONT_NORMAL, textSize_, { 255, 255, 255, 255 });
		SDL_QueryTexture(currStr, NULL, NULL, &strBounds.w, &strBounds.h);
		yOffset -= strBounds.h;
		strBounds.x = bounds.x + 5;
		strBounds.y = bounds.y + bounds.h + yOffset;
		SDL_RenderCopy(_renderer, currStr, NULL, &strBounds);
		SDL_DestroyTexture(currStr);

		// for each notification
		for (int i = 0; i < chatLog_->getLength(); i++) {
			// create texture, set bounds
			currStr = loadString(*chatLog_->getObjectAt(i), FONT_NORMAL, textSize_, { 255, 255, 255, 255 });
			SDL_QueryTexture(currStr, NULL, NULL, &strBounds.w, &strBounds.h);
			yOffset -= strBounds.h;
			strBounds.x = bounds.x + 5;
			strBounds.y = bounds.y + bounds.h + yOffset;

			// set fade
			currTime = *chatLogTiming_->getObjectAt(i);
			if (currTime < 1000)
				SDL_SetTextureAlphaMod(currStr, currTime / 4);
			else if (currTime <= 0)
				SDL_SetTextureAlphaMod(currStr, 0);

			// render and destroy
			SDL_RenderCopy(_renderer, currStr, NULL, &strBounds);
			SDL_DestroyTexture(currStr);
		}
	}
}

std::string ChatDisplay::getInput() {
	return inputText_;
}

void ChatDisplay::clearInput() {
	inputText_ = "";
}

void ChatDisplay::addInputChar(char c) {
	if (!active_) {
		if (c == 'T' || c == 't') {
			active_ = true;
		}
	} else { // if active
		if (c == 127) { // backspace
			if (inputText_.length() > 0)
				inputText_.pop_back();
		} else if (c == 13) { // enter/return
			if (inputText_.length() > 0) {
				Message m;
				m.type = MSGTYPE_CHAT;
				strncpy_s(m.text, DEFAULT_CHATSIZE, inputText_.c_str(), DEFAULT_CHATSIZE);
				_connectionManager->sendMessage(m);
			}

			clearInput();
			setActive(false);
		} else if (c == 27) { // escape
			clearInput();
			setActive(false);
		} else if (inputText_.length() < DEFAULT_CHATSIZE - 1) {
			inputText_ += c;
		}
	}
}

void ChatDisplay::addMessage(std::string msg) {
	chatLog_->addFirst(new std::string(msg));
	chatLogTiming_->addFirst(new int(5000));

	if (chatLog_->getLength() > logSize_) {
		delete chatLogTiming_->removeLast();
		delete chatLog_->removeLast();
	}
}

bool ChatDisplay::isActive() {
	return active_;
}

void ChatDisplay::setActive(bool b) {
	active_ = b;
}

void ChatDisplay::tick(int ms) {
	Iterator<int*> it = chatLogTiming_->getIterator();
	while (it.hasNext()) {
		int* curr = it.next();
		*curr -= ms;
		if (*curr < 0)
			*curr = 0;
	}
}