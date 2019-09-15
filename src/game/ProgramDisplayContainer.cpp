#include "MiscUtil.h"
#include "ProgramDisplayContainer.h"
#include "Global.h"
#include "ResourceLoader.h"

ProgramDisplayContainer::ProgramDisplayContainer(ANCHOR a, int xDisp, int yDisp, int wid, int hei, GUIContainer* par)
    : GUIObject(a, xDisp, yDisp, wid, hei, par)
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
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 160);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(gRenderer, &bounds);

    // draw bounds
    if (debug) drawBounds();

    // draw the program icon
    Program* currProgram = gameScreen->getSelectedProgram();
    if (currProgram == NULL) return;
    SDL_SetTextureColorMod(dataContainer->program_core_100px,
                           currProgram->getColor(0), currProgram->getColor(1), currProgram->getColor(2));
    SDL_Rect progBound;
    progBound.x = bounds.x + 10;
    progBound.y = bounds.y + 10;
    progBound.w = bounds.h - 20;
    progBound.h = bounds.h - 20;
    SDL_RenderCopy(gRenderer, dataContainer->program_core_100px, NULL, &progBound);
    progBound.w = progBound.w*(9.0/10.0);
    progBound.h = progBound.h*(9.0/10.0);
    SDL_RenderCopy(gRenderer, dataContainer->program_icons[currProgram->getType()], NULL, &progBound);

    SDL_Texture* name = loadString(currProgram->getName());
    progBound.x = bounds.x + 100;
    SDL_QueryTexture(name, NULL, NULL, &progBound.w, &progBound.h);
    SDL_RenderCopy(gRenderer, name, NULL, &progBound);
    SDL_DestroyTexture(name);

    std::string healthStr = "Health: " + to_string(currProgram->getHealth()) + "/" + to_string(currProgram->getMaxHealth());
    SDL_Texture* health = loadString(healthStr);
    progBound.y += progBound.h;
    SDL_QueryTexture(health, NULL, NULL, &progBound.w, &progBound.h);
    SDL_RenderCopy(gRenderer, health, NULL, &progBound);
    SDL_DestroyTexture(health);

    std::string moveStr = "Moves: " + to_string(currProgram->getMoves()) + "/" + to_string(currProgram->getMaxMoves());
    SDL_Texture* moves = loadString(moveStr);
    progBound.y += progBound.h;
    SDL_QueryTexture(moves, NULL, NULL, &progBound.w, &progBound.h);
    SDL_RenderCopy(gRenderer, moves, NULL, &progBound);
    SDL_DestroyTexture(moves);

    Iterator<ProgramAction*> it = currProgram->getActions()->getIterator();
    int row = 0;
    while (it.hasNext())
    {
        SDL_Texture* newTex = loadString(it.next()->name);
        SDL_QueryTexture(newTex, NULL, NULL, &progBound.w, &progBound.h);
        progBound.x = 100;
        progBound.y = 5+row;
        SDL_RenderCopy(gRenderer, newTex, NULL, &progBound);
        SDL_DestroyTexture(newTex);
        row += progBound.h;
    }
}

void ProgramDisplayContainer::setTransparency(int a)
{
    currAlpha = a;
}

bool ProgramDisplayContainer::mouseDown()
{
    return true;
}

bool ProgramDisplayContainer::mouseUp()
{
    return true;
}

void ProgramDisplayContainer::tick(int ms)
{

}

void ProgramDisplayContainer::resetBounds()
{
    recomputePosition();
}
