#include "Standard.h"
#include "PlayerDisplayContainer.h"

#include "Global.h"
#include "Player.h"
#include "GUIContainer.h"
#include "Client.h"
#include "Game.h"
#include "ResourceLoader.h"
#include "MiscUtil.h"
#include "DataContainer.h"
#include "GameScreen.h"
#include "ClientMirror.h"

PlayerDisplayContainer::PlayerDisplayContainer(ANCHOR a, Coord disp, Coord dims, GUIContainer* par)
	: GUIContainer(a, disp, dims, par, { 120, 120, 120, 140 })
{
	//nothing
}

PlayerDisplayContainer::~PlayerDisplayContainer()
{
	//dtor
}

void PlayerDisplayContainer::draw()
{
	// draw the background
	GUIContainer::drawBkg();

	// initialize constant
	int fontSize = 30;

	// adjust the size of this container
	int height = 5 + (fontSize + 5) * (client->getGame()->getHumanPlayers()->getLength() + client->getGame()->getAIPlayers()->getLength() + 2);

	// draw the container itself
	GUIContainer::drawBkg();

	// draw bounds
	if (debug >= DEBUG_NORMAL) drawBounds();

	// initialize locals
	int xOffset = 5;
	int yOffset = 5;
	SDL_Rect objBounds;

	// draw the human player section title
	SDL_Texture* humanPTitle = loadString("Human Players:", FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
	SDL_QueryTexture(humanPTitle, NULL, NULL, &objBounds.w, &objBounds.h);
	objBounds.x = bounds.x + xOffset;
	objBounds.y = bounds.y + yOffset;
	SDL_RenderCopy(gRenderer, humanPTitle, NULL, &objBounds);
	SDL_DestroyTexture(humanPTitle);
	yOffset += objBounds.h + 5;

	// draw each human player's display box
	Iterator<Player*> it = client->getGame()->getHumanPlayers()->getIterator();
	while (it.hasNext())
	{
		// draw background color box
		Player* curr = it.next();
		objBounds.x = bounds.x + xOffset;
		objBounds.y = bounds.y + yOffset;
		objBounds.w = 190;
		objBounds.h = fontSize;
		SDL_SetRenderDrawColor(gRenderer, curr->getColor().r, curr->getColor().g, curr->getColor().b, 150);
		SDL_RenderFillRect(gRenderer, &objBounds);

		// draw player ID (or client name)
		ClientMirror* cMirr = client->getClientMirrorByPlayerID(curr->getPlayerID());
		SDL_Texture* str = loadString(cMirr->name_, FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
		SDL_QueryTexture(str, NULL, NULL, &objBounds.w, &objBounds.h);
		objBounds.x = bounds.x + xOffset;
		objBounds.y = bounds.y + yOffset;
		SDL_RenderCopy(gRenderer, str, NULL, &objBounds);
		SDL_DestroyTexture(str);
		xOffset += objBounds.w + 5;

		// draw icons representing current state

		// draw icon if player has no programs on the board
		if (curr->getProgList()->getLength() <= 0)
		{
			SDL_QueryTexture(dataContainer->game_icon_dead, NULL, NULL, &objBounds.w, &objBounds.h);
			objBounds.x = bounds.x + xOffset;
			objBounds.y = bounds.y + yOffset;
			SDL_RenderCopy(gRenderer, dataContainer->game_icon_dead, NULL, &objBounds);
			xOffset += objBounds.w + 5;
		}

		// draw icon if it is currently this player's turn
		if (curr == client->getGame()->getCurrTurnPlayer())
		{
			SDL_QueryTexture(dataContainer->game_icon_currTurn, NULL, NULL, &objBounds.w, &objBounds.h);
			objBounds.x = bounds.x + xOffset;
			objBounds.y = bounds.y + yOffset;
			SDL_RenderCopy(gRenderer, dataContainer->game_icon_currTurn, NULL, &objBounds);
			xOffset += objBounds.w + 5;
		}

		// draw icon if this player is the current owner of the server
		for (int i = 0; i < client->getClientList()->getLength(); i++)
		{
			ClientMirror* currMirr = client->getClientList()->getObjectAt(i);
			if (currMirr->owner_ && currMirr->player_ == curr)
			{
				SDL_QueryTexture(dataContainer->game_icon_owner, NULL, NULL, &objBounds.w, &objBounds.h);
				objBounds.x = bounds.x + xOffset;
				objBounds.y = bounds.y + yOffset;
				SDL_RenderCopy(gRenderer, dataContainer->game_icon_owner, NULL, &objBounds);
				xOffset += objBounds.w + 5;
				break;
			}
		}

		// change offsets
		yOffset += fontSize + 5;
		xOffset = 5;
	}

	// draw the human player section title
	SDL_Texture* AIPTitle = loadString("AI Players:", FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
	SDL_QueryTexture(AIPTitle, NULL, NULL, &objBounds.w, &objBounds.h);
	objBounds.x = bounds.x + xOffset;
	objBounds.y = bounds.y + yOffset;
	SDL_RenderCopy(gRenderer, AIPTitle, NULL, &objBounds);
	SDL_DestroyTexture(AIPTitle);
	yOffset += objBounds.h + 5;

	// draw each human player's display box
	Iterator<Player*> it2 = client->getGame()->getAIPlayers()->getIterator();
	while (it2.hasNext())
	{
		// draw background color box
		Player* curr = it2.next();
		objBounds.x = bounds.x + xOffset;
		objBounds.y = bounds.y + yOffset;
		objBounds.w = 190;
		objBounds.h = fontSize;
		SDL_SetRenderDrawColor(gRenderer, curr->getColor().r, curr->getColor().g, curr->getColor().b, 150);
		SDL_RenderFillRect(gRenderer, &objBounds);

		// draw player ID
		SDL_Texture* str = loadString(to_string(curr->getPlayerID()), FONT_NORMAL, fontSize, { 255, 255, 255, 255 });
		SDL_QueryTexture(str, NULL, NULL, &objBounds.w, &objBounds.h);
		objBounds.x = bounds.x + xOffset;
		objBounds.y = bounds.y + yOffset;
		SDL_RenderCopy(gRenderer, str, NULL, &objBounds);
		SDL_DestroyTexture(str);
		xOffset += objBounds.w + 5;

		// draw icons representing current state
		if (curr->getProgList()->getLength() <= 0)
		{
			SDL_QueryTexture(dataContainer->game_icon_dead, NULL, NULL, &objBounds.w, &objBounds.h);
			objBounds.x = bounds.x + xOffset;
			objBounds.y = bounds.y + yOffset;
			SDL_RenderCopy(gRenderer, dataContainer->game_icon_dead, NULL, &objBounds);
			xOffset += objBounds.w + 5;
		}
		if (curr == client->getGame()->getCurrTurnPlayer())
		{
			SDL_QueryTexture(dataContainer->game_icon_currTurn, NULL, NULL, &objBounds.w, &objBounds.h);
			objBounds.x = bounds.x + xOffset;
			objBounds.y = bounds.y + yOffset;
			SDL_RenderCopy(gRenderer, dataContainer->game_icon_currTurn, NULL, &objBounds);
			xOffset += objBounds.w + 5;
		}
		// if (owner == curr->getPlayerID())

		// change offsets
		yOffset += fontSize + 5;
		xOffset = 5;
	}
}
