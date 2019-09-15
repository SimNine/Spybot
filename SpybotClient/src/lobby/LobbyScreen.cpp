#include "Standard.h"
#include "LobbyScreen.h"

#include "Global.h"
#include "Client.h"
#include "GUIButton.h"
#include "Message.h"
#include "ResourceLoader.h"
#include "MiscUtil.h"
#include "MainScreen.h"
#include "ClientMirror.h"
#include "ChatDisplay.h"

LobbyScreen::LobbyScreen()
	: GUIContainer(ANCHOR_NORTHWEST, { 0, 0 }, { SCREEN_WIDTH, SCREEN_HEIGHT }, NULL, { 0, 0, 0, 0 })
{
	buildGUI();
}

LobbyScreen::~LobbyScreen()
{
	//dtor
}

void LobbyScreen::buildGUI()
{
	GUIButton* leaveLobbyButton = new GUIButton(ANCHOR_NORTHEAST, { -200, 10 }, "Leave Lobby", this, []() { lobbyScreen->leaveLobby(); });
	this->addObject(leaveLobbyButton);

	GUIButton* loadLevelButton = new GUIButton(ANCHOR_NORTHEAST, { -200, 80 }, "Load Level", this, []() {
		Message msg;
		msg.type = MSGTYPE_LOAD;
		msg.levelNum = 2;
		client->sendMessage(msg); });
	this->addObject(loadLevelButton);

	chatDisplay_ = new ChatDisplay(ANCHOR_SOUTHWEST, { 0, -500 }, { 800, 500 }, this, 19);
	this->addObject(chatDisplay_);
}

void LobbyScreen::draw()
{
	// clear the screen (black)
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 40, 0);
	SDL_RenderClear(gRenderer);

	// render contained GUIObjs
	GUIContainer::drawContents();

	// render each clientID
	Iterator<ClientMirror*> clients = client->getClientList()->getIterator();
	int yOffset = 0;
	int textSize = 24;
	while (clients.hasNext())
	{
		ClientMirror* currID = clients.next();
		SDL_Rect textBound;
		SDL_Texture* text = loadString(currID->name_ + " (" + to_string(currID->clientID_) + ")", FONT_BOLD, textSize, { 255, 255, 255, 255 });
		SDL_QueryTexture(text, NULL, NULL, &textBound.w, &textBound.h);
		textBound.x = bounds.x;
		textBound.y = bounds.y + yOffset;
		SDL_RenderCopy(gRenderer, text, NULL, &textBound);
		SDL_DestroyTexture(text);

		yOffset += textSize;
	}
}

void LobbyScreen::leaveLobby()
{
	// clean up the pipe on this end; sends a shutdown signal automatically
	client->disconnect();

	// clean up the lobby screen, return to main screen
	currScreen = mainScreen;
}

ChatDisplay* LobbyScreen::getChatDisplay()
{
	return chatDisplay_;
}