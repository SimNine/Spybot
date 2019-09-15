#include "Standard.h"
#include "ProgramInventoryButton.h"

#include "Global.h"
#include "DataContainer.h"
#include "GameScreen.h"

ProgramInventoryButton::ProgramInventoryButton(ANCHOR a, Coord disp, PROGRAM t, GUIContainer* p)
    : GUIObject(a, disp, {30, 30}, p)
{
    icon = dataContainer->program_icons[t];
    progType = t;
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