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
	// establish config container
	gameConfigContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 800, 400 }, _color_bkg_standard);
	gameConfigContainer_->setMovable(false);
	this->addObject(gameConfigContainer_);

	// connected player container
	gameConnectedPlayerContainer_ = new GUIContainer(gameConfigContainer_, ANCHOR_NORTHWEST, { 5, 5 }, { 300, 340 }, _color_bkg_standard);
	gameConfigContainer_->addObject(gameConnectedPlayerContainer_);

	GUITexture* gameConfigLabel = new GUITexture(gameConnectedPlayerContainer_, ANCHOR_NORTH, { 0, 5 }, "Connected players:", 30);
	gameConnectedPlayerContainer_->addObject(gameConfigLabel);

	// level select container
	GUIContainer* gameConfigLevelContainer = new GUIContainer(gameConfigContainer_, ANCHOR_NORTHWEST, { 310, 5 }, { 300, 340 }, _color_bkg_standard);
	gameConfigContainer_->addObject(gameConfigLevelContainer);

	GUITexture* gameConfigLevelLabel = new GUITexture(gameConfigLevelContainer, ANCHOR_NORTH, { 0, 5 }, "LEVELS:", 40);
	gameConfigLevelContainer->addObject(gameConfigLevelLabel);

	gameConfigLevelArray_ = new GUIButton(gameConfigLevelContainer, ANCHOR_NORTH, { 0, 45 }, "ARRAY", 40,
		[] () {
		Message m; 
		m.type = MSGTYPE_GAMECONFIG; 
		m.gameConfigType = MSGGAMECONFIGTYPE_LEVEL_ARRAY;
		_connectionManager->sendMessage(m);
	});
	gameConfigLevelContainer->addObject(gameConfigLevelArray_);

	gameConfigLevelHash_ = new GUIButton(gameConfigLevelContainer, ANCHOR_NORTH, { 0, 85 }, "HASH", 40,
		[] () {
		Message m;
		m.type = MSGTYPE_GAMECONFIG;
		m.gameConfigType = MSGGAMECONFIGTYPE_LEVEL_HASH;
		_connectionManager->sendMessage(m);
	});
	gameConfigLevelContainer->addObject(gameConfigLevelHash_);

	gameConfigLevelCross_ = new GUIButton(gameConfigLevelContainer, ANCHOR_NORTH, { 0, 125 }, "CROSS", 40,
		[] () {
		Message m;
		m.type = MSGTYPE_GAMECONFIG;
		m.gameConfigType = MSGGAMECONFIGTYPE_LEVEL_CROSS;
		_connectionManager->sendMessage(m);
	});
	gameConfigLevelContainer->addObject(gameConfigLevelCross_);

	gameConfigLevelShowdown_ = new GUIButton(gameConfigLevelContainer, ANCHOR_NORTH, { 0, 165 }, "SHOWDOWN", 40,
		[] () {
		Message m;
		m.type = MSGTYPE_GAMECONFIG;
		m.gameConfigType = MSGGAMECONFIGTYPE_LEVEL_SHOWDOWN;
		_connectionManager->sendMessage(m);
	});
	gameConfigLevelContainer->addObject(gameConfigLevelShowdown_);

	// game mode container
	GUIContainer* gameConfigModeContainer = new GUIContainer(gameConfigContainer_, ANCHOR_NORTHWEST, { 615, 5 }, { 180, 340 }, _color_bkg_standard);
	gameConfigContainer_->addObject(gameConfigModeContainer);

	GUITexture* gameConfigModeLabel = new GUITexture(gameConfigModeContainer, ANCHOR_NORTH, { 0, 5 }, "Game Mode", 30);
	gameConfigModeContainer->addObject(gameConfigModeLabel);

	gameConfigButtonCoop_ = new GUIButton(gameConfigModeContainer, ANCHOR_NORTH, { 0, 40 }, { 83, 83 },
		[] () { 
		Message m; 
		m.type = MSGTYPE_GAMECONFIG; 
		m.gameConfigType = MSGGAMECONFIGTYPE_COOP; 
		_connectionManager->sendMessage(m); },
		_lobby_button_gamemode_coop,
		_lobby_button_gamemode_coop_over);
	gameConfigModeContainer->addObject(gameConfigButtonCoop_);

	gameConfigButtonFFA_ = new GUIButton(gameConfigModeContainer, ANCHOR_NORTH, { 0, 128 }, { 83, 83 },
		[] () { 
		Message m; 
		m.type = MSGTYPE_GAMECONFIG; 
		m.gameConfigType = MSGGAMECONFIGTYPE_FFA; 
		_connectionManager->sendMessage(m); },
		_lobby_button_gamemode_ffa,
		_lobby_button_gamemode_ffa_over);
	gameConfigModeContainer->addObject(gameConfigButtonFFA_);

	gameConfigButtonTeamDM_ = new GUIButton(gameConfigModeContainer, ANCHOR_NORTH, { 0, 216 }, { 83, 83 },
		[] () { 
		Message m; 
		m.type = MSGTYPE_GAMECONFIG; 
		m.gameConfigType = MSGGAMECONFIGTYPE_TEAMDM; 
		_connectionManager->sendMessage(m); },
		_lobby_button_gamemode_teamdm,
		_lobby_button_gamemode_teamdm_over);
	gameConfigModeContainer->addObject(gameConfigButtonTeamDM_);

	gameConfigButtonSelected_ = gameConfigButtonFFA_;

	// leave lobby button
	GUIButton* leaveLobbyButton = new GUIButton(gameConfigContainer_, ANCHOR_SOUTHWEST, { 5, -5 }, "LEAVE LOBBY", 45, 
		[] () {
		_lobbyOverlay->leaveLobby();
	});
	gameConfigContainer_->addObject(leaveLobbyButton);

	// start game button
	GUIButton* startGameButton = new GUIButton(gameConfigContainer_, ANCHOR_SOUTHEAST, { -5, -5 }, "START GAME", 45,
		[] () {
		Message m;
		m.type = MSGTYPE_STARTGAME;
		_connectionManager->sendMessage(m);
	});
	gameConfigContainer_->addObject(startGameButton);

	// chat display
	chatDisplay_ = new ChatDisplay(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 800, 500 }, 19);
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
}