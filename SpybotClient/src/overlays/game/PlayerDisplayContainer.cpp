#include "Standard.h"
#include "PlayerDisplayContainer.h"

#include "Global.h"
#include "PlayerMirror.h"
#include "GUIContainer.h"
#include "Client.h"
#include "GameMirror.h"
#include "ResourceLoader.h"
#include "MiscUtil.h"
#include "Data.h"
#include "GameOverlay.h"
#include "ClientMirror.h"
#include "TeamMirror.h"
#include "ConnectionManager.h"
#include "GUIButton.h"

PlayerDisplayContainer::PlayerDisplayContainer(ANCHOR a, Coord disp, Coord dims, GUIContainer* par)
	: GUIContainer(par, a, disp, dims, _color_bkg_standard) {
	this->addObject(new GUIButton(this, ANCHOR_NORTHEAST, { -10, 10 }, "MINIMIZE", 20,
		[]() {
		_gameOverlay->hidePlayerDisp();
	}));
}

PlayerDisplayContainer::~PlayerDisplayContainer() {
	//dtor
}

void PlayerDisplayContainer::draw() {
	// initialize constant
	int fontSize = 30;

	// adjust the size of this container
	int numTeams = _client->getGame()->getAllTeams()->getLength();
	int numPlayers = 0;
	Iterator<TeamMirror*> itTeams = _client->getGame()->getAllTeams()->getIterator();
	while (itTeams.hasNext()) {
		numPlayers += itTeams.next()->getAllPlayers()->getLength();
	}
	int height = 5 + (fontSize + 5) * (numTeams + numPlayers);
	this->setDimensions({ this->getDimensions().x, height });

	// draw the container itself
	GUIContainer::draw();

	// draw bounds
	if (_debug >= DEBUG_NORMAL)
		drawBounds();

	// initialize locals
	int xOffset = 5;
	int yOffset = 5;
	SDL_Rect objBounds;

	// for each team
	itTeams = _client->getGame()->getAllTeams()->getIterator();
	while (itTeams.hasNext()) {
		// get current team
		TeamMirror* currTeam = itTeams.next();

		// draw this team's number
		SDL_Texture* humanPTitle = loadString("Team " + to_string(currTeam->getTeamID()) + ":", FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
		SDL_QueryTexture(humanPTitle, NULL, NULL, &objBounds.w, &objBounds.h);
		objBounds.x = bounds_.x + xOffset;
		objBounds.y = bounds_.y + yOffset;
		SDL_SetTextureAlphaMod(humanPTitle, currAlpha_);
		SDL_RenderCopy(_renderer, humanPTitle, NULL, &objBounds);
		SDL_DestroyTexture(humanPTitle);
		yOffset += fontSize + 5;

		// draw each player's display box
		Iterator<PlayerMirror*> it = currTeam->getAllPlayers()->getIterator();
		while (it.hasNext()) {
			// draw background color box
			PlayerMirror* curr = it.next();
			objBounds.x = bounds_.x + xOffset;
			objBounds.y = bounds_.y + yOffset;
			objBounds.w = 190;
			objBounds.h = fontSize;
			SDL_SetRenderDrawColor(_renderer, curr->getColor().r, curr->getColor().g, curr->getColor().b, Uint8(150.0*(currAlpha_/255.0)));
			SDL_RenderFillRect(_renderer, &objBounds);

			// draw player ID (or client name)
			ClientMirror* cMirr = _connectionManager->getClientMirrorByPlayerID(curr->getPlayerID());
			SDL_Texture* str;
			if (cMirr == NULL)
				str = loadString(to_string(curr->getPlayerID()), FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
			else
				str = loadString(cMirr->name_, FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
			SDL_QueryTexture(str, NULL, NULL, &objBounds.w, &objBounds.h);
			objBounds.x = bounds_.x + xOffset;
			objBounds.y = bounds_.y + yOffset;
			SDL_SetTextureAlphaMod(str, currAlpha_);
			SDL_RenderCopy(_renderer, str, NULL, &objBounds);
			SDL_DestroyTexture(str);
			xOffset += objBounds.w + 5;

			// draw icons representing current state

			// draw icon if this player is an AI or a human
			SDL_Texture* playerType = NULL;
			if (curr->getAI()) {
				playerType = _game_icon_ai;
			} else {
				playerType = _game_icon_human;
			}
			SDL_QueryTexture(playerType, NULL, NULL, &objBounds.w, &objBounds.h);
			objBounds.x = bounds_.x + xOffset;
			objBounds.y = bounds_.y + yOffset;
			SDL_SetTextureAlphaMod(playerType, currAlpha_);
			SDL_RenderCopy(_renderer, playerType, NULL, &objBounds);
			xOffset += objBounds.w + 5;

			// draw icon if this player is the current owner of the server
			if (_connectionManager->getServerOwner() == NULL) {
				// log("no server owner, for some reason\n");
			} else if (curr == _connectionManager->getServerOwner()->player_) {
				SDL_QueryTexture(_game_icon_owner, NULL, NULL, &objBounds.w, &objBounds.h);
				objBounds.x = bounds_.x + xOffset;
				objBounds.y = bounds_.y + yOffset;
				SDL_SetTextureAlphaMod(_game_icon_owner, currAlpha_);
				SDL_RenderCopy(_renderer, _game_icon_owner, NULL, &objBounds);
				xOffset += objBounds.w + 5;
			}

			// draw icon if player has no programs on the board
			if (curr->getProgList()->getLength() <= 0) {
				SDL_QueryTexture(_game_icon_dead, NULL, NULL, &objBounds.w, &objBounds.h);
				objBounds.x = bounds_.x + xOffset;
				objBounds.y = bounds_.y + yOffset;
				SDL_SetTextureAlphaMod(_game_icon_dead, currAlpha_);
				SDL_RenderCopy(_renderer, _game_icon_dead, NULL, &objBounds);
				xOffset += objBounds.w + 5;
			}

			// draw icon if it is currently this player's turn
			if (curr == _client->getGame()->getCurrTurnPlayer()) {
				SDL_QueryTexture(_game_icon_currTurn, NULL, NULL, &objBounds.w, &objBounds.h);
				objBounds.x = bounds_.x + xOffset;
				objBounds.y = bounds_.y + yOffset;
				SDL_SetTextureAlphaMod(_game_icon_currTurn, currAlpha_);
				SDL_RenderCopy(_renderer, _game_icon_currTurn, NULL, &objBounds);
				xOffset += objBounds.w + 5;
			}

			// change offsets
			yOffset += fontSize + 5;
			xOffset = 5;
		}
	}
}
