#include "Standard.h"
#include "LobbyOverlay.h"

#include "Global.h"
#include "Client.h"
#include "GUIButton.h"
#include "Message.h"
#include "ResourceLoader.h"
#include "MiscUtil.h"
#include "MainOverlay.h"
#include "ClientMirror.h"
#include "ChatDisplay.h"
#include "Data.h"
#include "GUITexture.h"
#include "ConnectionManager.h"
#include "BackgroundOverlay.h"

LobbyOverlay::LobbyOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_clear) {
	buildGUI();
}

LobbyOverlay::~LobbyOverlay() {
	//dtor
}

void LobbyOverlay::buildGUI() {
	GUIButton* leaveLobbyButton = new GUIButton(this, ANCHOR_NORTHEAST, { -10, 10 }, "Leave Lobby", 50, [] () {
		_lobbyOverlay->leaveLobby();
	});
	this->addObject(leaveLobbyButton);

	gameConfigContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 800, 400 }, _color_bkg_standard);
	gameConnectedPlayerContainer_ = new GUIContainer(gameConfigContainer_, ANCHOR_NORTHWEST, { 5, 5 }, { 300, 390 }, _color_bkg_standard);
	GUITexture* gameConfigLabel = new GUITexture(gameConnectedPlayerContainer_, ANCHOR_NORTH, { 0, 5 }, "Connected players:", 30);
	gameConnectedPlayerContainer_->addObject(gameConfigLabel);
	gameConfigContainer_->addObject(gameConnectedPlayerContainer_);
	GUIContainer* gameConfigModeContainer = new GUIContainer(gameConfigContainer_, ANCHOR_NORTHEAST, { -5, 5 }, { 150, 390 }, _color_bkg_standard);
	GUITexture* gameConfigModeLabel = new GUITexture(gameConfigModeContainer, ANCHOR_NORTH, { 0, 5 }, "Game Mode", 30);
	gameConfigModeContainer->addObject(gameConfigModeLabel);
	gameConfigButtonCoop_ = new GUIButton(gameConfigModeContainer, ANCHOR_NORTH, { 0, 40 }, { 83, 83 },
		[] () { Message m; m.type = MSGTYPE_GAMECONFIG; m.gameConfigType = MSGGAMECONFIGTYPE_COOP; _connectionManager->sendMessage(m); },
		_lobby_button_gamemode_coop,
		_lobby_button_gamemode_coop_over);
	gameConfigModeContainer->addObject(gameConfigButtonCoop_);
	gameConfigButtonFFA_ = new GUIButton(gameConfigModeContainer, ANCHOR_NORTH, { 0, 128 }, { 83, 83 },
		[] () { Message m; m.type = MSGTYPE_GAMECONFIG; m.gameConfigType = MSGGAMECONFIGTYPE_FFA; _connectionManager->sendMessage(m); },
		_lobby_button_gamemode_ffa,
		_lobby_button_gamemode_ffa_over);
	gameConfigModeContainer->addObject(gameConfigButtonFFA_);
	gameConfigButtonTeamDM_ = new GUIButton(gameConfigModeContainer, ANCHOR_NORTH, { 0, 216 }, { 83, 83 },
		[] () { Message m; m.type = MSGTYPE_GAMECONFIG; m.gameConfigType = MSGGAMECONFIGTYPE_TEAMDM; _connectionManager->sendMessage(m); },
		_lobby_button_gamemode_teamdm,
		_lobby_button_gamemode_teamdm_over);
	gameConfigModeContainer->addObject(gameConfigButtonTeamDM_);
	gameConfigButtonSelected_ = gameConfigButtonFFA_;
	gameConfigContainer_->addObject(gameConfigModeContainer);
	gameConfigContainer_->setMovable(false);
	this->addObject(gameConfigContainer_);

	chatDisplay_ = new ChatDisplay(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 800, 500 }, 19);
	//this->addObject(chatDisplay_);
}

void LobbyOverlay::draw() {
	// render contained GUIObjs
	GUIContainer::drawContents();
	chatDisplay_->draw();

	// render each clientID
	Iterator<ClientMirror*> clients = _connectionManager->getClientList()->getIterator();
	int xOffset = 5;
	int yOffsetDefault = 50;
	int yOffset = yOffsetDefault;
	int textSize = 24;
	SDL_Rect configBounds = gameConnectedPlayerContainer_->getBounds();
	while (clients.hasNext()) {
		ClientMirror* currID = clients.next();
		SDL_Rect textBound;
		SDL_Texture* text = loadString(currID->name_ + " (" + to_string(currID->clientID_) + ")", FONT_BOLD, textSize, _color_white);
		SDL_QueryTexture(text, NULL, NULL, &textBound.w, &textBound.h);
		textBound.x = configBounds.x + xOffset;
		textBound.y = configBounds.y + yOffset;
		SDL_RenderCopy(_renderer, text, NULL, &textBound);
		SDL_DestroyTexture(text);

		yOffset += textSize;
		if (yOffset > configBounds.h) {
			yOffset = yOffsetDefault;
			xOffset += 100;
		}
	}

	// render highlight for selected gamemode
	if (gameConfigButtonSelected_ != NULL) {
		SDL_Rect tempRect = gameConfigButtonSelected_->getBounds();
		SDL_Texture* buttonSelected = loadTexture(_lobby_button_gamemode_selected);
		SDL_RenderCopy(_renderer, buttonSelected, NULL, &tempRect);
		SDL_DestroyTexture(buttonSelected);
	}
}

void LobbyOverlay::tick(int ms) {
	GUIContainer::tick(ms);
	chatDisplay_->tick(ms);
}

void LobbyOverlay::resetBounds() {
	GUIContainer::resetBounds();
	chatDisplay_->resetBounds();
}

void LobbyOverlay::leaveLobby() {
	// clean up the pipe on this end; sends a shutdown signal automatically
	_connectionManager->disconnect();

	// clean up the lobby screen, return to main screen
	_overlayStack->removeAll();
	_overlayStack->push(_backgroundOverlay);
	_overlayStack->push(_mainOverlay);
}

ChatDisplay* LobbyOverlay::getChatDisplay() {
	return chatDisplay_;
}

void LobbyOverlay::setGameMode(GAMEMODE gameMode) {
	if (gameMode == GAMEMODE_COOP)
		gameConfigButtonSelected_ = gameConfigButtonCoop_;
	else if (gameMode == GAMEMODE_FFA)
		gameConfigButtonSelected_ = gameConfigButtonFFA_;
	else if (gameMode == GAMEMODE_TEAMDM)
		gameConfigButtonSelected_ = gameConfigButtonTeamDM_;

	gameMode_ = gameMode;
}