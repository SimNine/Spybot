#include "Standard.h"
#include "ProgramDisplayContainer.h"

#include "Global.h"
#include "Program.h"
#include "ResourceLoader.h"
#include "GameScreen.h"
#include "Data.h"
#include "MiscUtil.h"
#include "ProgramAction.h"
#include "Client.h"
#include "Player.h"
#include "Game.h"

ProgramDisplayContainer::ProgramDisplayContainer(ANCHOR a, Coord disp, Coord dims, GUIContainer* par)
    : GUIContainer(a, disp, dims, par, _color_bkg_standard)
{
    //nothing
}

ProgramDisplayContainer::~ProgramDisplayContainer()
{
    //dtor
}

void ProgramDisplayContainer::draw()
{
    // draw the box
    GUIContainer::drawBkg();

    // draw bounds
    if (_debug >= DEBUG_NORMAL) 
		drawBounds();

    // get the current program
	Program* currProgram = _client->getPlayer()->getSelectedProgram();
    if (currProgram == NULL) 
		return;

    // draw the current program's name
    SDL_Rect nameBound;
    SDL_Texture* name = loadString(currProgram->getName(), FONT_BOLD, 24, {255, 255, 255, 255});
    SDL_QueryTexture(name, NULL, NULL, &nameBound.w, &nameBound.h);
    nameBound.x = bounds.x + bounds.w/2 - nameBound.w/2;
    nameBound.y = bounds.y - nameBound.h;
    SDL_RenderCopy(_renderer, name, NULL, &nameBound);
    SDL_DestroyTexture(name);

    // draw the program icon
    SDL_Rect iconBound = {bounds.x + 10, bounds.y + 10, 100, 100};
	SDL_Color c = currProgram->getOwner()->getColor();
	SDL_SetTextureColorMod(_program_core_100px, c.r, c.g, c.b);
    SDL_RenderCopy(_renderer, _program_core_100px, NULL, &iconBound);
    iconBound.w = (int)(iconBound.w*(9.0/10.0));
    iconBound.h = (int)(iconBound.h*(9.0/10.0));
    SDL_RenderCopy(_renderer, _program_icons[currProgram->getType()], NULL, &iconBound);

    // draw stats
    SDL_Rect statBound = iconBound;
    std::string healthStr = "Health: " + to_string(currProgram->getHealth()) + "/" + to_string(currProgram->getMaxHealth());
    SDL_Texture* health = loadString(healthStr, FONT_BOLD, 24, {255, 255, 255, 255});
    statBound.y = iconBound.y + 100;
    SDL_QueryTexture(health, NULL, NULL, &statBound.w, &statBound.h);
    SDL_RenderCopy(_renderer, health, NULL, &statBound);
    SDL_DestroyTexture(health);

    std::string moveStr = "Moves: " + to_string(currProgram->getMoves()) + "/" + to_string(currProgram->getMaxMoves());
    SDL_Texture* moves = loadString(moveStr, FONT_BOLD, 24, {255, 255, 255, 255});
    statBound.y += statBound.h;
    SDL_QueryTexture(moves, NULL, NULL, &statBound.w, &statBound.h);
    SDL_RenderCopy(_renderer, moves, NULL, &statBound);
    SDL_DestroyTexture(moves);

    std::string actionStr = "Actions: " + to_string(currProgram->getActionsLeft()) + "/" + to_string(1);
    SDL_Texture* actions = loadString(actionStr, FONT_BOLD, 24, {255, 255, 255, 255});
    statBound.y += statBound.h;
    SDL_QueryTexture(actions, NULL, NULL, &statBound.w, &statBound.h);
    SDL_RenderCopy(_renderer, actions, NULL, &statBound);
    SDL_DestroyTexture(actions);

    // draw actions
    SDL_Rect actionBound = bounds;
    actionBound.x += 120;
    actionBound.y += 10;

    std::string actionStr2 = "Actions:";
    SDL_Texture* actions2 = loadString(actionStr2, FONT_BOLD, 30, {255, 255, 255, 255});
    SDL_QueryTexture(actions2, NULL, NULL, &actionBound.w, &actionBound.h);
    SDL_RenderCopy(_renderer, actions2, NULL, &actionBound);
    SDL_DestroyTexture(actions2);

    Iterator<ProgramAction*> it = currProgram->getActions()->getIterator();
    while (it.hasNext())
    {
        actionBound.y += actionBound.h;
        SDL_Texture* newTex = loadString(it.next()->name, FONT_NORMAL, 24, {255, 255, 255, 255});
        SDL_QueryTexture(newTex, NULL, NULL, &actionBound.w, &actionBound.h);
        SDL_RenderCopy(_renderer, newTex, NULL, &actionBound);
        SDL_DestroyTexture(newTex);
    }
}
