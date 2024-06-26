#include "Standard.h"
#include "GameOverlay.h"

#include "Global.h"
#include "Client.h"
#include "Message.h"
#include "ResourceLoader.h"
#include "GUIButton.h"
#include "Data.h"
#include "GUITexture.h"
#include "ProgramDisplayContainer.h"
#include "ProgramInventoryDisplay.h"
#include "PlayerDisplayContainer.h"
#include "GameMirror.h"
#include "PlayerMirror.h"
#include "ProgramMirror.h"
#include "ProgramActionMirror.h"
#include "MapOverlay.h"
#include "ChatDisplay.h"
#include "ClientMirror.h"
#include "Animation.h"
#include "ConnectionManager.h"
#include "MiscUtil.h"
#include "GUIEffectFade.h"
#include "SpawnGroupMirror.h"
#include "GUIEffectTranslate.h"

GameOverlay::GameOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_black) {
	buildGUI();

	bkgImg_ = _game_backgrounds[0];
	programViewTeams_ = false;
	programViewPlayers_ = true;
	textureTickCount_ = 0;
	bkgPos_ = { 0, 0 };
	shiftSpeed_ = 0.15;

	animList_ = new LinkedList<Animation*>();
}

GameOverlay::~GameOverlay() {
	//dtor
}

void GameOverlay::buildGUI() {

	/*
	WIN/ENDGAME CONTAINER
	*/

	winMenu_ = new GUIContainer(NULL, ANCHOR_CENTER, { 0, 0 }, { _screenWidth, _screenHeight }, _color_bkg_standard);
	winMenu_->setTransparency(0);
	this->addObject(winMenu_);

	winningTeam_ = new GUITexture(this, ANCHOR_NORTH, { 0, 50 }, { 400, 100 }, _game_button_aiStep);
	winMenu_->addObject(winningTeam_);

	GUIContainer* winMenuOptions_ = new GUIContainer(winMenu_, ANCHOR_CENTER, { 0, 0 }, { 200, 200 }, _color_bkg_standard);
	winMenu_->addObject(winMenuOptions_);
	GUIButton* winMenuOptionReturn = new GUIButton(winMenuOptions_, ANCHOR_CENTER, { 0, 0 }, "RETURN TO MAP", 30,
		[] () {
		_gameOverlay->hideWinContainer();
		_overlayStack->removeAll();
		_overlayStack->push(_mapOverlay);
	});
	winMenuOptions_->addObject(winMenuOptionReturn);

	/*
	CURRENT TURN DISPLAY
	*/

	currTurn_ = new GUITexture(this, ANCHOR_NORTH, { 0, 50 }, { 400, 100 }, _game_button_aiStep);
	currTurn_->setTransparency(0);
	addObject(currTurn_);

	/*
	CREDIT PICKUP CONTAINER
	*/

	creditPickupContainer_ = new GUIContainer(this, ANCHOR_SOUTH, { 0, -100 }, { 200, 50 }, _color_bkg_standard);
	creditPickupContainer_->setTransparency(0);
	this->addObject(creditPickupContainer_);

	creditPickupTexture_ = NULL;

	/*
	CREDIT COUNTER CONTAINER
	*/

	creditCounterContainer_ = new GUIContainer(this, ANCHOR_NORTH, { 0, 10 }, { 500, 50 }, _color_bkg_standard);
	this->addObject(creditCounterContainer_);

	creditCounterContainer_->addObject(new GUITexture(creditCounterContainer_, ANCHOR_WEST, { 10, 0 }, { 30, 30 }, _item_icons[ITEM_CREDIT]));

	creditCounterText_ = NULL;

	/*
	DEBUG OPTIONS CONTAINER
	*/

	debugOptions_ = new GUIContainer(this, ANCHOR_NORTHWEST, { 10, 10 }, { 220, 190 }, _color_bkg_standard);
	debugOptions_->setTransparency(0);
	this->addObject(debugOptions_);

	GUIButton* teamViewButton = new GUIButton(debugOptions_, ANCHOR_NORTHWEST, { 10, 10 }, { 200, 50 },
		[] () {_gameOverlay->toggleViewTeams(); },
		_game_button_viewTeams);
	debugOptions_->addObject(teamViewButton);
	GUIButton* winGameButton = new GUIButton(debugOptions_, ANCHOR_NORTHWEST, { 10, 70 }, { 200, 50 },
		[] () {Message m; m.type = MSGTYPE_INFO; m.infoType = MSGINFOTYPE_GAMESTATUS; m.statusType = GAMESTATUS_END; _connectionManager->sendMessage(m); },
		_game_button_win);
	debugOptions_->addObject(winGameButton);
	GUIButton* stepButton = new GUIButton(debugOptions_, ANCHOR_NORTHWEST, { 10, 130 }, { 200, 50 },
		[] () {Message msg; msg.type = MSGTYPE_AISTEP;	_connectionManager->sendMessage(msg); },
		_game_button_aiStep);
	debugOptions_->addObject(stepButton);

	/*
	END TURN BUTTON
	*/

	turnButton_ = new GUIButton(this, ANCHOR_SOUTHEAST, { -10, -10 }, { 200, 50 },
		[] () {
		Message msg;
		msg.type = MSGTYPE_NEXTTURN;
		msg.playerID = _client->getPlayer()->getPlayerID();
		_connectionManager->sendMessage(msg); },
		_game_button_endTurn);
	turnButton_->setTransparency(0);
	this->addObject(turnButton_);

	/*
	PLAYER DISPLAY WINDOW
	*/

	playerTab_ = new GUIContainer(this, ANCHOR_SOUTHEAST, { -190, -50 }, { 150, 50 }, _color_bkg_standard);
	this->addObject(playerTab_);

	GUIButton* playerExpandButton = new GUIButton(playerTab_, ANCHOR_CENTER, { 0,0 }, "PLAYER LIST", 30,
		[]() {
		_gameOverlay->showPlayerDisp();
	});
	playerTab_->addObject(playerExpandButton);

	playerDisp_ = new PlayerDisplayContainer(ANCHOR_NORTHWEST, { 10, 10 }, { 200, 200 }, this);
	playerDisp_->setMovable(true);
	this->addObject(playerDisp_);

	/*
	PROGRAM DISPLAY WINDOW
	*/

	progDisp_ = new ProgramDisplayContainer(this, ANCHOR_NORTHEAST, { -10, 10 }, { 300, 400 });
	progDisp_->setTransparency(0);
	progDisp_->setMovable(true);
	this->addObject(progDisp_);

	/*
	START GAME BUTTON
	*/

	startGameButton_ = new GUIButton(this, ANCHOR_SOUTHWEST, { 10, 10 }, "START GAME", 50,
		[] () {
		Message m;
		m.type = MSGTYPE_INFO;
		m.infoType = MSGINFOTYPE_GAMESTATUS;
		m.statusType = GAMESTATUS_PLAYING;
		_connectionManager->sendMessage(m);
	});
	this->addObject(startGameButton_);

	/*
	PROGRAM INVENTORY DISPLAY
	*/

	progInvTab_ = new GUIContainer(this, ANCHOR_SOUTHEAST, { -20, -50 }, { 150, 50 }, _color_bkg_standard);
	this->addObject(progInvTab_);

	GUIButton* progInvExpandButton = new GUIButton(progInvTab_, ANCHOR_CENTER, { 0,0 }, "PROGRAM LIST", 30, 
		[]() {
		_gameOverlay->showProgInv();
	});
	progInvTab_->addObject(progInvExpandButton);

	progInv_ = new ProgramInventoryDisplay(this, ANCHOR_NORTHEAST, { -20, 20 }, { 400, 600 });
	progInv_->setMovable(true);
	this->addObject(progInv_);

	/*
	CHAT DISPLAY
	*/

	chatDisplay_ = new ChatDisplay(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 800, 500 }, 19);

	/*
	PAUSE MENU CONTAINER
	*/

	pauseMenu_ = new GUIContainer(NULL, ANCHOR_CENTER, { 0, 0 }, { _screenWidth, _screenHeight }, _color_bkg_standard);
	pauseMenu_->setTransparency(0);
	this->addObject(pauseMenu_);

	GUITexture* pauseText = new GUITexture(pauseMenu_, ANCHOR_CENTER, { -200, -200 }, "PAUSED", 150);
	pauseMenu_->addObject(pauseText);

	GUIContainer* pauseMenuOptions_ = new GUIContainer(pauseMenu_, ANCHOR_CENTER, { 0, 0 }, { 220, 4 * 60 + 10 }, _color_bkg_standard);
	pauseMenu_->addObject(pauseMenuOptions_);
	GUIButton* resumeGameButton = new GUIButton(pauseMenuOptions_, ANCHOR_NORTHWEST, { 10, 10 }, { 200, 50 },
		[] () {
		_gameOverlay->hidePauseMenu();
	}, _game_button_resume);
	pauseMenuOptions_->addObject(resumeGameButton);
	GUIButton* exitToMapButton = new GUIButton(pauseMenuOptions_, ANCHOR_NORTHWEST, { 10, 70 }, { 200, 50 },
		[] () {
		_gameOverlay->hidePauseMenu();
		_overlayStack->removeAll();
		_overlayStack->push(_mapOverlay);
	}, _game_button_quitToMap);
	pauseMenuOptions_->addObject(exitToMapButton);
	GUIButton* exitToMainButton = new GUIButton(pauseMenuOptions_, ANCHOR_NORTHWEST, { 10, 130 }, { 200, 50 },
		[] () {
		_gameOverlay->hidePauseMenu();
		_overlayStack->removeAll();
		_overlayStack->push(_mapOverlay);
	}, _game_button_quitToMain);
	pauseMenuOptions_->addObject(exitToMainButton);
	GUIButton* exitToDesktopButton = new GUIButton(pauseMenuOptions_, ANCHOR_NORTHWEST, { 10, 190 }, { 200, 50 },
		[] () {
		_quit = true;
	}, _game_button_quitToDesktop);
	pauseMenuOptions_->addObject(exitToDesktopButton);
}

void GameOverlay::resetBounds() {
	GUIContainer::resetBounds();
	chatDisplay_->resetBounds();

	if (!canShift())
		centerScreen();
}

bool GameOverlay::mouseDown() {
	if (GUIContainer::mouseDown())
		return true;

	Coord click = { (bkgPos_.x + _mousePos.x) / _tileWidth, (bkgPos_.y + _mousePos.y) / _tileWidth };

	if (_client->getGame()->getStatus() == GAMESTATUS_PREGAME) {
		Message msg;
		msg.type = MSGTYPE_SELECT;
		msg.pos = click;
		msg.selectType = MSGSELECTTYPE_TILE;
		_connectionManager->sendMessage(msg);
	} else if (_client->getGame()->getStatus() == GAMESTATUS_PLAYING) {
		if ((_client->getPlayer()->getSelectedProgram() == NULL || !_client->getPlayer()->canSelectedProgramMoveTo(click)) &&
			(_client->getPlayer()->getSelectedAction() == NULL || !(_client->getPlayer()->getSelectedActionDist(click) > 0))) {
			Message msg;
			msg.type = MSGTYPE_SELECT;
			msg.pos = click;
			msg.selectType = MSGSELECTTYPE_TILE;
			_connectionManager->sendMessage(msg);
		} else { // if (game->getPlayer()->canSelectedProgramMoveTo(x, y))
			if (_client->getPlayer()->getSelectedAction() == NULL) {
				Message msg;
				msg.type = MSGTYPE_MOVE;
				msg.pos = click;
				msg.playerID = _client->getPlayer()->getPlayerID();
				msg.programID = _client->getPlayer()->getSelectedProgram()->getProgramID();
				_connectionManager->sendMessage(msg);
			} else if (((_client->getPlayer()->getSelectedAction()->type_ == ACTIONTYPE_TILEPLACE && !_client->getGame()->isTiled(click) && _client->getPlayer()->getSelectedActionDistAll(click) > 0) ||
				(_client->getPlayer()->getSelectedActionDist(click) > 0 && _client->getPlayer()->getSelectedAction()->type_ != ACTIONTYPE_TILEPLACE)) &&
				_client->getPlayer()->getSelectedProgram()->getActionsLeft() > 0) {
				Message msg;
				msg.type = MSGTYPE_ACTION;
				msg.pos = click;
				msg.playerID = _client->getPlayer()->getPlayerID();
				msg.programID = _client->getPlayer()->getSelectedProgram()->getProgramID();
				_connectionManager->sendMessage(msg);
			}
		}

		return false;
	}

	return true;
}

bool GameOverlay::mouseUp() {
	return GUIContainer::mouseUp();
}

void GameOverlay::drawBkg() {
	// draw background image
	SDL_RenderCopy(_renderer, bkgImg_, NULL, NULL);
}

void GameOverlay::drawGrid() {
	if (_client->getGame() == NULL) {
		log("CLIENT ERR: doing GameOverlay::drawGrid() without a game in session\n");
		return;
	}

	// pointer for convenience
	GameMirror* g = _client->getGame();
	//PlayerMirror* currTurnPlayer = g->getCurrTurnPlayer();
	//ProgramActionMirror* selectedAction = ();
	// TODO: find out why TILEPLACE action candidates are not showing

	// set temp variables
	Coord topLeft = { bkgPos_.x / _tileWidth, bkgPos_.y / _tileWidth };
	Coord bottomRight;
	bottomRight.x = topLeft.x + _screenWidth / _tileWidth + 1;
	bottomRight.y = topLeft.y + _screenHeight / _tileWidth + 1;

	// check for overflows
	if (bottomRight.x > _boardWidth)
		bottomRight.x = _boardWidth;
	if (bottomRight.y > _boardHeight)
		bottomRight.y = _boardHeight;
	if (topLeft.x < 0)
		topLeft.x = 0;
	if (topLeft.y < 0)
		topLeft.y = 0;

	// draw grid
	SDL_Rect tileRect;
	for (int x = topLeft.x; x < bottomRight.x; x++) {
		for (int y = topLeft.y; y < bottomRight.y; y++) {
			Coord curr = { x, y };

			// if there's no tile to be drawn here
			if (g->getTileAt(curr) == TILE_NONE)
				continue;

			// default position of a tile,
			// assuming it is 28x28 px
			int xDefault = 4 + _tileWidth*curr.x - bkgPos_.x;
			int yDefault = 4 + _tileWidth*curr.y - bkgPos_.y;
			int sizeDefault = _tileWidth - 4;
			tileRect.x = xDefault;
			tileRect.y = yDefault;
			tileRect.w = sizeDefault;
			tileRect.h = sizeDefault;

			// if there's a program at this tile
			if (g->getProgramAt(curr) != NULL) {
				// get this program
				ProgramMirror* prog = g->getProgramAt(curr);

				// draw this program's tile
				tileRect.x = xDefault - 1;
				tileRect.y = yDefault - 1;
				tileRect.w = sizeDefault + 2;
				tileRect.h = sizeDefault + 2;
				SDL_SetTextureAlphaMod(_program_core, 255);
				if (programViewPlayers_) {
					SDL_Color c = prog->getOwner()->getColor();
					SDL_SetTextureColorMod(_program_core, c.r, c.g, c.b);
				} else if (programViewTeams_) {
					if (prog->getTeamID() == 0)
						SDL_SetTextureColorMod(_program_core, 0, 0, prog->getColor(2));
					else if (prog->getTeamID() == 1)
						SDL_SetTextureColorMod(_program_core, prog->getColor(0), 0, 0);
					else if (prog->getTeamID() == 2)
						SDL_SetTextureColorMod(_program_core, 0, prog->getColor(1), 0);
					else if (prog->getTeamID() == 3)
						SDL_SetTextureColorMod(_program_core, prog->getColor(0), prog->getColor(1), 0);
					else
						SDL_SetTextureColorMod(_program_core, 0, prog->getColor(1), prog->getColor(2));
				} else
					SDL_SetTextureColorMod(_program_core, prog->getColor(0), prog->getColor(1), prog->getColor(2));

				// if this is the farthest chunk of this program
				if (g->getCurrTurnPlayer() != NULL &&
					prog == g->getCurrTurnPlayer()->getSelectedProgram() &&
					prog->getHealth() == prog->getMaxHealth() &&
					prog->getMoves() > 0) {
					Coord currTail = prog->getTail();
					if (curr.x == currTail.x && curr.y == currTail.y) {
						if (textureTickCount_ % 100 < 50)
							SDL_RenderCopy(_renderer, _program_core, NULL, &tileRect);
					} else
						SDL_RenderCopy(_renderer, _program_core, NULL, &tileRect);
				} else SDL_RenderCopy(_renderer, _program_core, NULL, &tileRect);

				// if this is part of the selected program, indicate it
				if (g->getCurrTurnPlayer() != NULL &&
					prog == g->getCurrTurnPlayer()->getSelectedProgram() &&
					_debug >= DEBUG_NORMAL) {
					SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
					SDL_RenderDrawLine(_renderer, tileRect.x, tileRect.y, tileRect.x + tileRect.w, tileRect.y + tileRect.h);
					SDL_RenderDrawLine(_renderer, tileRect.x + tileRect.w, tileRect.y, tileRect.x, tileRect.y + tileRect.h);
				}

				// draw the bridges from this program's tile to adjacent tiles
				if (g->getProgramAt({ curr.x, curr.y + 1 }) == prog) {
					tileRect.x = xDefault + 9;
					tileRect.y = yDefault + 26;
					tileRect.w = 10;
					tileRect.h = 5;
					if (programViewPlayers_) {
						SDL_Color c = prog->getOwner()->getColor();
						SDL_SetTextureColorMod(_program_core_vertical, c.r, c.g, c.b);
					} else if (programViewTeams_) {
						if (prog->getTeamID() == 0)
							SDL_SetTextureColorMod(_program_core_vertical, 0, 0, prog->getColor(2));
						else if (prog->getTeamID() == 1)
							SDL_SetTextureColorMod(_program_core_vertical, prog->getColor(0), 0, 0);
						else if (prog->getTeamID() == 2)
							SDL_SetTextureColorMod(_program_core_vertical, 0, prog->getColor(1), 0);
						else if (prog->getTeamID() == 3)
							SDL_SetTextureColorMod(_program_core_vertical, prog->getColor(0), prog->getColor(1), 0);
						else
							SDL_SetTextureColorMod(_program_core_vertical, 0, prog->getColor(1), prog->getColor(2));
					} else
						SDL_SetTextureColorMod(_program_core_vertical, prog->getColor(0), prog->getColor(1), prog->getColor(2));
					SDL_RenderCopy(_renderer, _program_core_vertical, NULL, &tileRect);
				}
				if (g->getProgramAt({ curr.x + 1, curr.y }) == prog) {
					tileRect.x = xDefault + 26;
					tileRect.y = yDefault + 8;
					tileRect.w = 5;
					tileRect.h = 12;
					if (programViewPlayers_) {
						SDL_Color c = prog->getOwner()->getColor();
						SDL_SetTextureColorMod(_program_core_horizontal, c.r, c.g, c.b);
					} else if (programViewTeams_) {
						if (prog->getTeamID() == 0)
							SDL_SetTextureColorMod(_program_core_horizontal, 0, 0, prog->getColor(2));
						else if (prog->getTeamID() == 1)
							SDL_SetTextureColorMod(_program_core_horizontal, prog->getColor(0), 0, 0);
						else if (prog->getTeamID() == 2)
							SDL_SetTextureColorMod(_program_core_horizontal, 0, prog->getColor(1), 0);
						else if (prog->getTeamID() == 3)
							SDL_SetTextureColorMod(_program_core_horizontal, prog->getColor(0), prog->getColor(1), 0);
						else
							SDL_SetTextureColorMod(_program_core_horizontal, 0, prog->getColor(1), prog->getColor(2));
					} else
						SDL_SetTextureColorMod(_program_core_horizontal, prog->getColor(0), prog->getColor(1), prog->getColor(2));
					SDL_RenderCopy(_renderer, _program_core_horizontal, NULL, &tileRect);
				}

				// draw the icon IF this is the core tile
				if (prog->getHead().x == curr.x && prog->getHead().y == curr.y) {
					tileRect.x = xDefault - 1;
					tileRect.y = yDefault - 1;
					tileRect.w = 27;
					tileRect.h = 27;
					SDL_SetTextureAlphaMod(_program_icons[prog->getType()], 255);
					SDL_RenderCopy(_renderer, _program_icons[prog->getType()], NULL, &tileRect);

					// draw the highlight rectangle if this program is selected
					if (g->getCurrTurnPlayer() != NULL &&
						g->getCurrTurnPlayer() &&
						prog == g->getCurrTurnPlayer()->getSelectedProgram()) {
						tileRect.x = xDefault - 2;
						tileRect.y = yDefault - 2;
						tileRect.w = _tileWidth;
						tileRect.h = _tileWidth;
						SDL_Color c = g->getCurrTurnPlayer()->getColor();
						SDL_SetTextureColorMod(_tile_selected, c.r, c.g, c.b);
						SDL_SetTextureAlphaMod(_tile_selected, (Uint8)((double)-textureTickCount_ / 1000.0) * 255 + 255);
						SDL_RenderCopy(_renderer, _tile_selected, NULL, &tileRect);
					}

					// draw the checkmark if this program is done for the turn
					if (prog->isDone()) {
						tileRect.x = xDefault + 17;
						tileRect.y = yDefault;
						tileRect.w = 10;
						tileRect.h = 10;
						SDL_RenderCopy(_renderer, _game_icon_checkmark, NULL, &tileRect);
					}
				}
			} else { // if there is no program on this tile
				TILE tile = g->getTileAt(curr);
				SDL_Texture* tileImg = _tile_images[tile];
				if (tile == TILE_NONE)
					continue;

				if (tile == TILE_SPAWN || tile == TILE_SPAWN2) {
					SpawnGroupMirror* group = g->getSpawnGroupAt(curr);
					if (group == NULL) {
						SDL_SetTextureColorMod(tileImg, 255, 255, 255);
					} else if (group->getPlayerID() != -1) {
						PlayerMirror* sp = g->getPlayerByID(group->getPlayerID());
						SDL_SetTextureColorMod(tileImg, sp->getColor().r, sp->getColor().g, sp->getColor().b);
					} else {
						SDL_SetTextureColorMod(tileImg, 50, 50, 50);
					}
				}

				SDL_QueryTexture(tileImg, NULL, NULL, &tileRect.w, &tileRect.h);
				tileRect.x = xDefault - (tileRect.w - 28) / 2;
				tileRect.y = yDefault - (tileRect.h - 28) / 2;
				SDL_RenderCopy(_renderer, tileImg, NULL, &tileRect);
			}

			// if the mouse is over this tile
			if (_mousePos.x - tileRect.x > 0 &&
				_mousePos.x - (tileRect.x + tileRect.w) < 0 &&
				_mousePos.y - tileRect.y > 0 &&
				_mousePos.y - (tileRect.y + tileRect.h) < 0 &&
				!pauseMenu_->isVisible()) {
				tileRect.x = xDefault;
				tileRect.y = yDefault;
				tileRect.w = sizeDefault;
				tileRect.h = sizeDefault;
				SDL_SetTextureAlphaMod(_tile_over, 125);
				SDL_RenderCopy(_renderer, _tile_over, NULL, &tileRect);
			}

			// if there is an item on this tile
			if (g->getItemAt(curr) != ITEM_NONE) {
				SDL_QueryTexture(_item_icons[g->getItemAt(curr)], NULL, NULL, &tileRect.w, &tileRect.h);
				tileRect.x = xDefault - (tileRect.w - 28) / 2;
				tileRect.y = yDefault - (tileRect.h - 28) / 2;
				SDL_RenderCopy(_renderer, _item_icons[g->getItemAt(curr)], NULL, &tileRect);
			}

			// if this is a selected tile
			Iterator<ClientMirror*> playIt = _connectionManager->getClientList()->getIterator();
			while (playIt.hasNext()) {
				PlayerMirror* pCurr = playIt.next()->player_;
				if (curr == pCurr->getSelectedTile()) {
					tileRect.x = xDefault - 2;
					tileRect.y = yDefault - 2;
					tileRect.w = _tileWidth;
					tileRect.h = _tileWidth;
					SDL_SetTextureColorMod(_tile_selected, pCurr->getColor().r, pCurr->getColor().g, pCurr->getColor().b);
					SDL_SetTextureAlphaMod(_tile_selected, (Uint8)((double)-textureTickCount_ / 1000.0) * 255 + 255);
					SDL_RenderCopy(_renderer, _tile_selected, NULL, &tileRect);
				}
			}

			// if this tile is movable-to by the current player
			if (g->getCurrTurnPlayer() != NULL &&
				g->getCurrTurnPlayer()->getSelectedProgramDist(curr) > 0 &&
				g->getCurrTurnPlayer()->getSelectedProgram()->getOwner() == g->getCurrTurnPlayer() &&
				g->getCurrTurnPlayer()->getSelectedAction() == NULL) {
				tileRect.x = xDefault;
				tileRect.y = yDefault;
				tileRect.w = sizeDefault;
				tileRect.h = sizeDefault;

				if (curr == g->getCurrTurnPlayer()->getSelectedProgram()->getHead() + Coord{ 0, 1 })
					SDL_RenderCopy(_renderer, _tile_moveSouth, NULL, &tileRect);
				else if (curr == g->getCurrTurnPlayer()->getSelectedProgram()->getHead() + Coord{ 1, 0 })
					SDL_RenderCopy(_renderer, _tile_moveEast, NULL, &tileRect);
				else if (curr == g->getCurrTurnPlayer()->getSelectedProgram()->getHead() + Coord{ 0, -1 })
					SDL_RenderCopy(_renderer, _tile_moveNorth, NULL, &tileRect);
				else if (curr == g->getCurrTurnPlayer()->getSelectedProgram()->getHead() + Coord{ -1, 0 })
					SDL_RenderCopy(_renderer, _tile_moveWest, NULL, &tileRect);
				else
					SDL_RenderCopy(_renderer, _tile_movePossible, NULL, &tileRect);
			}

			// if this tile is in the range of a programAction
			if (g->getCurrTurnPlayer() != NULL &&
				g->getProgramAt(curr) != g->getCurrTurnPlayer()->getSelectedProgram() &&
				g->getCurrTurnPlayer()->getSelectedAction() != NULL) {
				tileRect.x = xDefault - 2;
				tileRect.y = yDefault - 2;
				tileRect.w = sizeDefault + 4;
				tileRect.h = sizeDefault + 4;

				if (g->getCurrTurnPlayer()->getSelectedAction()->type_ == ACTIONTYPE_TILEPLACE &&
					!g->isTiled(curr) &&
					g->getCurrTurnPlayer()->getSelectedActionDistAll(curr) > 0) {
					SDL_RenderCopy(_renderer, _tile_actionHeal, NULL, &tileRect);
				} else if (g->getCurrTurnPlayer()->getSelectedActionDist(curr) > 0 &&
					g->getCurrTurnPlayer()->getSelectedAction()->type_ != ACTIONTYPE_TILEPLACE) {
					switch (g->getCurrTurnPlayer()->getSelectedAction()->type_) {
					case ACTIONTYPE_DAMAGE:
						SDL_RenderCopy(_renderer, _tile_actionDamage, NULL, &tileRect);
						break;
					case ACTIONTYPE_HEAL:
						SDL_RenderCopy(_renderer, _tile_actionHeal, NULL, &tileRect);
						break;
					case ACTIONTYPE_MAXHEALTHDOWN:
						SDL_RenderCopy(_renderer, _tile_actionHeal, NULL, &tileRect);
						break;
					case ACTIONTYPE_MAXHEALTHUP:
						SDL_RenderCopy(_renderer, _tile_actionHeal, NULL, &tileRect);
						break;
					case ACTIONTYPE_SPEEDDOWN:
						SDL_RenderCopy(_renderer, _tile_actionSpeed, NULL, &tileRect);
						break;
					case ACTIONTYPE_SPEEDUP:
						SDL_RenderCopy(_renderer, _tile_actionHeal, NULL, &tileRect);
						break;
					case ACTIONTYPE_TILEDELETE:
						SDL_RenderCopy(_renderer, _tile_actionHeal, NULL, &tileRect);
						break;
					case ACTIONTYPE_TILEPLACE:
						SDL_RenderCopy(_renderer, _tile_actionHeal, NULL, &tileRect);
						break;
					case ACTIONTYPE_MAXACTIONSUP:
						SDL_RenderCopy(_renderer, _tile_actionSpeed, NULL, &tileRect);
						break;
					case ACTIONTYPE_MAXACTIONSDOWN:
						SDL_RenderCopy(_renderer, _tile_actionSpeed, NULL, &tileRect);
						break;
					case ACTIONTYPE_TRANSMIT:
						SDL_RenderCopy(_renderer, _tile_actionTransport, NULL, &tileRect);
						break;
					case ACTIONTYPE_TELEPORT:
						SDL_RenderCopy(_renderer, _tile_actionTransport, NULL, &tileRect);
						break;
					case ACTIONTYPE_FRAGMENT:
						SDL_RenderCopy(_renderer, _tile_actionTransport, NULL, &tileRect);
						break;
					default:
						break;
					}
				}
			}
		}
	}

	// draw all animation effects
	Iterator<Animation*> it = animList_->getIterator();
	while (it.hasNext()) {
		Animation* currAnim = it.next();
		int xPos = 4 + _tileWidth*currAnim->getPos().x - bkgPos_.x;
		int yPos = 4 + _tileWidth*currAnim->getPos().y - bkgPos_.y;
		currAnim->draw({ xPos + _tileWidth / 2, yPos + _tileWidth / 2 });
	}

	// draw board bounding rectangle
	if (_debug >= DEBUG_NORMAL) {
		tileRect.x = -bkgPos_.x + 4;
		tileRect.y = -bkgPos_.y + 4;
		tileRect.w = _tileWidth * _boardWidth - 4;
		tileRect.h = _tileWidth * _boardHeight - 4;
		SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 0);
		SDL_RenderDrawRect(_renderer, &tileRect);

		SDL_RenderDrawLine(_renderer, -bkgPos_.x + (_boardWidth / 2)*_tileWidth, -bkgPos_.y + 4, -bkgPos_.x + (_boardWidth / 2)*_tileWidth, -bkgPos_.y + _boardHeight*_tileWidth); // vert
		SDL_RenderDrawLine(_renderer, -bkgPos_.x + 4, -bkgPos_.y + (_boardHeight / 2)*_tileWidth, -bkgPos_.x + _boardWidth*_tileWidth, -bkgPos_.y + (_boardHeight / 2)*_tileWidth); // horiz

		tileRect.x = -bkgPos_.x + g->getLeftBound()*_tileWidth;
		tileRect.y = -bkgPos_.y + g->getTopBound()*_tileWidth;
		tileRect.w = (g->getRightBound() - g->getLeftBound())*_tileWidth;
		tileRect.h = (g->getBottomBound() - g->getTopBound())*_tileWidth;
		SDL_RenderDrawRect(_renderer, &tileRect);
	}
}

void GameOverlay::draw() {
	drawBkg();
	drawGrid();
	GUIContainer::drawContents();
	chatDisplay_->draw();
}

void GameOverlay::shiftBkg(Coord disp) {
	if (!canShift())
		return;

	if (bkgPos_.x + disp.x + _screenWidth / 2 < _client->getGame()->getLeftBound()*_tileWidth)
		bkgPos_.x = _client->getGame()->getLeftBound()*_tileWidth - _screenWidth / 2;
	else if (bkgPos_.x + disp.x + _screenWidth / 2 > _client->getGame()->getRightBound()*_tileWidth)
		bkgPos_.x = _client->getGame()->getRightBound()*_tileWidth - _screenWidth / 2;
	else
		bkgPos_.x += disp.x;

	if (bkgPos_.y + disp.y + _screenHeight / 2 < _client->getGame()->getTopBound()*_tileWidth)
		bkgPos_.y = _client->getGame()->getTopBound()*_tileWidth - _screenHeight / 2;
	else if (bkgPos_.y + disp.y + _screenHeight / 2 > _client->getGame()->getBottomBound()*_tileWidth)
		bkgPos_.y = _client->getGame()->getBottomBound()*_tileWidth - _screenHeight / 2;
	else
		bkgPos_.y += disp.y;
}

void GameOverlay::toggleViewTeams() {
	programViewTeams_ = !programViewTeams_;
}

void GameOverlay::setBackgroundImg(BACKGROUND b) {
	bkgImg_ = _game_backgrounds[b];
}

bool GameOverlay::canShift() {
	if (_client->getGame() == NULL)
		return false;
	else if ((_client->getGame()->getRightBound() - _client->getGame()->getLeftBound())*_tileWidth < _screenWidth - 200 &&
		(_client->getGame()->getBottomBound() - _client->getGame()->getTopBound())*_tileWidth < _screenHeight - 200)
		return false;
	else
		return true;
}

void GameOverlay::tick(int ms) {
	// tick gui objects
	GUIContainer::tick(ms);
	chatDisplay_->tick(ms);

	// don't do anything if pause is visible
	if (pauseMenu_->isVisible())
		return;

	// test to activate/deactivate the debug options
	if (_debug >= DEBUG_MINIMAL)
		debugOptions_->setTransparency(255);
	else
		debugOptions_->setTransparency(0);

	// adjust time-dependent textures
	textureTickCount_ += ms;
	if (textureTickCount_ >= 1000)
		textureTickCount_ = 0;

	// fade out the playerTurn display if it is visible
	if (currTurn_->isVisible() && currTurn_->getTransparency() == 255)
		currTurn_->addEffect(new GUIEffectFade(0, 500, currTurn_->getTransparency(), 0));

	// tick all animations
	Iterator<Animation*> it = animList_->getIterator();
	while (it.hasNext())
		it.next()->tick(ms);

	// check for, and remove, dead animations
	int anim = 0;
	while (anim < animList_->getLength()) {
		Animation* currAnim = animList_->getObjectAt(anim);
		if (currAnim->isDead()) {
			animList_->remove(currAnim);
			delete currAnim;
		} else
			anim++;
	}
	
	//if (_debug >= DEBUG_NORMAL)
		//log("num anims: " + to_string(animList_->getLength()) + "\n");

	// check if the current music track is done, if so, pick a new one
	if (Mix_PlayingMusic() == 0) {
		int rand_1 = rand() % 4;
		int rand_2 = rand() % 2;
		switch (rand_1) {
		case 0:
			Mix_PlayMusic(_music_game1, rand_2);
			break;
		case 1:
			Mix_PlayMusic(_music_game2, rand_2);
			break;
		case 2:
			Mix_PlayMusic(_music_game3, rand_2);
			break;
		case 3:
			Mix_PlayMusic(_music_game4, rand_2);
			break;
		}
	}

	// scan for keys currently pressed
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	int shiftAmt = (int)(shiftSpeed_* (double)ms);
	if (currentKeyStates[SDL_SCANCODE_UP])
		shiftBkg({ 0, -shiftAmt });
	else if (currentKeyStates[SDL_SCANCODE_DOWN])
		shiftBkg({ 0, shiftAmt });

	if (currentKeyStates[SDL_SCANCODE_LEFT])
		shiftBkg({ -shiftAmt, 0 });
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		shiftBkg({ shiftAmt, 0 });

	// if the mouse is at an edge, try to shift the background
	if (_mousePos.x < 20)
		shiftBkg({ -shiftAmt, 0 });
	else if (_mousePos.x > _screenWidth - 20)
		shiftBkg({ shiftAmt, 0 });

	if (_mousePos.y < 20)
		shiftBkg({ 0, -shiftAmt });
	else if (_mousePos.y > _screenHeight - 20)
		shiftBkg({ 0, shiftAmt });
}

void GameOverlay::centerScreen() {
	if (_client == NULL || _client->getGame() == NULL) {
		bkgPos_.x = 100;
		bkgPos_.y = 100;
	} else {
		bkgPos_.x = (_client->getGame()->getRightBound() + _client->getGame()->getLeftBound())*_tileWidth / 2 - _screenWidth / 2;
		bkgPos_.y = (_client->getGame()->getBottomBound() + _client->getGame()->getTopBound())*_tileWidth / 2 - _screenHeight / 2;
	}
}

void GameOverlay::changeGameStatus(GAMESTATUS g) {
	// TODO: refactor this
	switch (g) {
	case GAMESTATUS_PREGAME:
		this->refreshCreditCounter();
		startGameButton_->setTransparency(255);
		turnButton_->setTransparency(0);
		progInv_->setTransparency(255);
		progInv_->updateContents();
		break;
	case GAMESTATUS_PLAYING:
		startGameButton_->setTransparency(0);
		turnButton_->setTransparency(255);
		progInv_->setTransparency(0);
		break;
	case GAMESTATUS_END:
		break;
	}
}

ChatDisplay* GameOverlay::getChatDisplay() {
	return chatDisplay_;
}

void GameOverlay::setPlayerTurnDisplay(std::string name) {
	SDL_Texture* nameTex = loadString((name + "'s turn"), FONT_NORMAL, 100, { 255, 255, 255, 255 });
	SDL_DestroyTexture(currTurn_->swapTexture(nameTex));
	int wid = 0;
	int hei = 0;
	SDL_QueryTexture(nameTex, NULL, NULL, &wid, &hei);
	currTurn_->setDimensions({ wid, hei });
	currTurn_->addEffect(new GUIEffectFade(0, 500, currTurn_->getTransparency(), 255));
}

void GameOverlay::showPauseMenu() {
	pauseMenu_->setTransparency(255);
}

void GameOverlay::hidePauseMenu() {
	pauseMenu_->setTransparency(0);
}

void GameOverlay::showProgInv() {
	progInv_->setDisplacement({ -20, 20 });
	progInv_->resetBounds();
	progInv_->updateContents();
	progInv_->addEffect(new GUIEffectFade(0, 500, 0, 255));
	progInvTab_->addEffect(new GUIEffectTranslate(0, 500, { -20, 0 }, { -20, 50 }));
	progInvTab_->addEffect(new GUIEffectFade(0, 500, 255, 0));
}

void GameOverlay::hideProgInv() {
	progInv_->addEffect(new GUIEffectFade(0, 500, 255, 0));
	progInvTab_->addEffect(new GUIEffectTranslate(0, 500, { -20, 50 }, { -20, 0 }));
	progInvTab_->addEffect(new GUIEffectFade(0, 500, 0, 255));
}

void GameOverlay::showPlayerDisp() {
	playerDisp_->setDisplacement({ 20, 20 });
	playerDisp_->resetBounds();
	playerDisp_->addEffect(new GUIEffectFade(0, 500, 0, 255));
	playerTab_->addEffect(new GUIEffectTranslate(0, 500, { -190, 0 }, { -190, 50 }));
	playerTab_->addEffect(new GUIEffectFade(0, 500, 255, 0));
}

void GameOverlay::hidePlayerDisp() {
	playerDisp_->addEffect(new GUIEffectFade(0, 500, 255, 0));
	playerTab_->addEffect(new GUIEffectTranslate(0, 500, { -190, 50 }, { -190, 0 }));
	playerTab_->addEffect(new GUIEffectFade(0, 500, 0, 255));
}

void GameOverlay::toggleTurnButtonShown(bool b) {
	if (b)
		turnButton_->setTransparency(255);
	else
		turnButton_->setTransparency(0);
}

void GameOverlay::addAnimation(Animation* a) {
	animList_->addFirst(a);
}

void GameOverlay::updateProgramInventoryDisplay() {
	progInv_->updateContents();
}

void GameOverlay::showWinContainer(int teamID) {
	winMenu_->removeObject(winningTeam_);
	delete winningTeam_;
	winningTeam_ = new GUITexture(winMenu_, ANCHOR_NORTH, { 0, 100 }, "TEAM " + to_string(teamID) + " WINS!", 100);
	winMenu_->addObject(winningTeam_);

	winMenu_->addEffect(new GUIEffectFade(0, 1000, 0, 255));
	turnButton_->addEffect(new GUIEffectFade(0, 1000, 255, 0));
	playerDisp_->addEffect(new GUIEffectFade(0, 1000, 255, 0));
	progDisp_->addEffect(new GUIEffectFade(0, 1000, 255, 0));
	creditCounterContainer_->addEffect(new GUIEffectFade(0, 1000, 255, 0));
}

void GameOverlay::hideWinContainer() {
	winMenu_->setTransparency(0);
}

void GameOverlay::showCreditPickup(int numCredits) {
	if (creditPickupTexture_ != NULL) {
		creditPickupContainer_->removeObject(creditPickupTexture_);
		delete creditPickupTexture_;
	}

	creditPickupTexture_ = new GUITexture(creditPickupContainer_, ANCHOR_CENTER, { 0, 0 }, to_string(numCredits) + " CREDITS ACQUIRED", 30);
	creditPickupContainer_->addObject(creditPickupTexture_);

	creditPickupContainer_->addEffect(new GUIEffectFade(0, 250, 0, 255));
	creditPickupContainer_->addEffect(new GUIEffectFade(2000, 250, 255, 0));
}

void GameOverlay::refreshCreditCounter() {
	if (creditCounterText_ != NULL) {
		creditCounterContainer_->removeObject(creditCounterText_);
		delete creditCounterText_;
	}
	creditCounterText_ = new GUITexture(creditCounterContainer_, ANCHOR_EAST, { -10, 0 }, to_string(_client->getMyClientMirror()->credits_), 30);
	creditCounterContainer_->addObject(creditCounterText_);

	creditCounterContainer_->setDimensions({ 60 + creditCounterText_->getDimensions().x, 50 });
	creditCounterContainer_->resetBounds();
}

ProgramDisplayContainer* GameOverlay::getProgramDisplayContainer() {
	return progDisp_;
}

void GameOverlay::startGameContainers() {
	winMenu_->setTransparency(0);
	turnButton_->setTransparency(255);
	progDisp_->setCurrProg(NULL);
	progDisp_->setTransparency(0);
	creditCounterContainer_->setTransparency(255);

	playerTab_->setTransparency(0);
	playerDisp_->setTransparency(255);

	progInvTab_->setTransparency(0);
	progInv_->setTransparency(255);
}