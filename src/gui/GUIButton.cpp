#include "GUIButton.h"
#include "Global.h"
#include "ResourceLoader.h"

#include <stdio.h>

GUIButton::GUIButton(ANCHOR a, int xDisp, int yDisp, std::string str, GUIContainer* parent, void (*func) (void))
    : GUIObject(a, xDisp, yDisp, 0, 0, parent)
{
    this->func = func;
    this->bkgNormal = loadString(str, FONT_NORMAL, 50, {255, 255, 255, 255});
    this->bkgOver = loadString(str, FONT_NORMAL, 50, {150, 150, 150, 255});
    this->bkgPressed = loadString(str, FONT_NORMAL, 50, {150, 150, 150, 150});;

    int w, h;
    SDL_QueryTexture(bkgNormal, NULL, NULL, &w, &h);
    setBounds(xDisp, yDisp, w, h);
}

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

    if (debug >= DEBUG_NORMAL) drawBounds();
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
    if (a > 255) currAlpha = 255;
    else if (a < 0) currAlpha = 0;
    else currAlpha = a;

    SDL_SetTextureAlphaMod(bkgNormal, currAlpha);
    SDL_SetTextureAlphaMod(bkgOver, currAlpha);
    SDL_SetTextureAlphaMod(bkgPressed, currAlpha);
}

void GUIButton::tick(int ms)
{
    fadeStep(ms);
    return;
}

void GUIButton::resetBounds()
{
    recomputePosition();
}
