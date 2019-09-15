#include "ProgramDisplayContainer.h"
#include "Global.h"
#include "ResourceLoader.h"

ProgramDisplayContainer::ProgramDisplayContainer(ANCHOR a, int xDisp, int yDisp, int wid, int hei, GUIContainer* par)
    : GUIObject(a, xDisp, yDisp, wid, hei, par)
{
    //ctor
}

ProgramDisplayContainer::~ProgramDisplayContainer()
{
    //dtor
}

void ProgramDisplayContainer::draw()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 160);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(gRenderer, &bounds);

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

    //SDL_Texture* moves = loadString("Moves:");
    //SDL_RenderCopy(gRenderer, moves, NULL, &progBound);
}

void ProgramDisplayContainer::setTransparency(int a)
{
    transparency = a;
}

bool ProgramDisplayContainer::mouseDown()
{
    return true;
}

bool ProgramDisplayContainer::mouseUp()
{
    return true;
}

void ProgramDisplayContainer::tick()
{

}
