#include "ProgramInventoryButton.h"
#include "Global.h"

#include <SDL.h>

ProgramInventoryButton::ProgramInventoryButton(ANCHOR a, int xD, int yD, int* progList, PROGRAM t, GUIContainer* p)
    : GUIObject(a, xD, yD, 30, 30, p)
{
    icon = dataContainer->program_icons[t];
    this->progList = progList;
    progType = t;

    //int w, h;
    //SDL_QueryTexture(icon, NULL, NULL, &w, &h);
    //setBounds(xD, yD, w, h);
}

ProgramInventoryButton::~ProgramInventoryButton()
{
    // nada
}

void ProgramInventoryButton::setTransparency(int a)
{
    startAlpha = a;
    endAlpha = a;
    currAlpha = a;
}

bool ProgramInventoryButton::mouseDown()
{
    currProgram = progType;
    gameScreen->tryPlacingProgram(currProgram);
    return true;
}

bool ProgramInventoryButton::mouseUp()
{
    return true;
}

void ProgramInventoryButton::draw()
{
    SDL_Rect r;
    r.w = 30;
    r.h = 30;
    r.x = bounds.x;
    r.y = bounds.y;
    SDL_RenderCopy(gRenderer, icon, NULL, &bounds);

    if (debug >= DEBUG_NORMAL) drawBounds();
}

void ProgramInventoryButton::tick(int t)
{

}

void ProgramInventoryButton::resetBounds()
{
    GUIObject::recomputePosition();
}
