#include "GUIButton.h"
#include "Global.h"
#include <stdio.h>

GUIButton::GUIButton(ANCHOR a, int xDisp, int yDisp, int width, int height, GUIContainer* parent,
                     void (*func) (void), SDL_Texture* bkgN, SDL_Texture* bkgO, SDL_Texture* bkgP)
    : GUIObject(a, xDisp, yDisp, width, height, parent)
{
    this->func = func;
    this->bkgNormal = bkgN;
    this->bkgOver = bkgO;
    this->bkgPressed = bkgP;
}

GUIButton::GUIButton(ANCHOR a, int xDisp, int yDisp, int width, int height, GUIContainer* parent,
                     void (*func) (void), SDL_Texture* bkgN, SDL_Texture* bkgO)
    : GUIObject(a, xDisp, yDisp, width, height, parent)
{
    this->func = func;
    this->bkgNormal = bkgN;
    this->bkgOver = bkgO;
    this->bkgPressed = bkgO;
}

GUIButton::GUIButton(ANCHOR a, int xDisp, int yDisp, int width, int height, GUIContainer* parent,
                     void (*func) (void), SDL_Texture* bkgN)
    : GUIObject(a, xDisp, yDisp, width, height, parent)
{
    this->func = func;
    this->bkgNormal = bkgN;
    this->bkgOver = bkgN;
    this->bkgPressed = bkgN;
}

GUIButton::GUIButton(ANCHOR a, int xDisp, int yDisp, int width, int height, GUIContainer* parent,
                     void (*func) (void))
    : GUIObject(a, xDisp, yDisp, width, height, parent)
{
    this->func = func;
    this->bkgNormal = NULL;
    this->bkgOver = NULL;
    this->bkgPressed = NULL;
}

GUIButton::~GUIButton()
{
    //dtor
}

void GUIButton::draw()
{
    if (mousePressed && isMouseOver() && bkgPressed != NULL)
    {
        SDL_RenderCopy(gRenderer, bkgPressed, NULL, &bounds);
    }
    else if (!mousePressed && isMouseOver() && bkgOver != NULL)
    {
        SDL_RenderCopy(gRenderer, bkgOver, NULL, &bounds);
    }
    else if (bkgNormal != NULL)
    {
        SDL_RenderCopy(gRenderer, bkgNormal, NULL, &bounds);
    }

    if (debug) drawBounds();
}

bool GUIButton::mouseDown()
{
    pressed = true;
    return true;
}

bool GUIButton::mouseUp()
{
    if (pressed) func();
    pressed = false;
    return true;
}

void GUIButton::setTransparency(int a)
{
    SDL_SetTextureAlphaMod(bkgNormal, a);
    SDL_SetTextureAlphaMod(bkgOver, a);
    SDL_SetTextureAlphaMod(bkgPressed, a);
}

void GUIButton::tick()
{
    return;
}
