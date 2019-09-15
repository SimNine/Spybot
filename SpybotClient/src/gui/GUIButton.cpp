#include "Standard.h"
#include "GUIButton.h"

#include "ResourceLoader.h"
#include "Global.h"

GUIButton::GUIButton(ANCHOR a, Coord disp, std::string str, GUIContainer* parent, void (*func) (void))
    : GUIObject(a, disp, {0, 0}, parent)
{
    this->func = func;
    this->bkgNormal = loadString(str, FONT_NORMAL, 50, {255, 255, 255, 255});
    this->bkgOver = loadString(str, FONT_NORMAL, 50, {150, 150, 150, 255});
    this->bkgPressed = loadString(str, FONT_NORMAL, 50, {150, 150, 150, 150});;

    int w, h;
    SDL_QueryTexture(bkgNormal, NULL, NULL, &w, &h);
    setBounds(disp, {w, h});
    willDestroyTextures = true;
}

GUIButton::GUIButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent,
                     void (*func) (void), std::string bkgN, std::string bkgO, std::string bkgP)
    : GUIObject(a, disp, dims, parent)
{
    this->func = func;
    this->bkgNormal = loadTexture(bkgN);
    this->bkgOver = loadTexture(bkgO);
    this->bkgPressed = loadTexture(bkgP);
    willDestroyTextures = true;
}

GUIButton::GUIButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent,
                     void (*func) (void), std::string bkgN, std::string bkgO)
    : GUIObject(a, disp, dims, parent)
{
    this->func = func;
    this->bkgNormal = loadTexture(bkgN);
    this->bkgOver = loadTexture(bkgO);
    this->bkgPressed = loadTexture(bkgO);
    willDestroyTextures = true;
}

GUIButton::GUIButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent,
                     void (*func) (void), std::string bkgN)
    : GUIObject(a, disp, dims, parent)
{
    this->func = func;
    this->bkgNormal = loadTexture(bkgN);
    this->bkgOver = loadTexture(bkgN);
    this->bkgPressed = loadTexture(bkgN);
    willDestroyTextures = true;
}

GUIButton::GUIButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent,
                     void (*func) (void), SDL_Texture* bkgN, SDL_Texture* bkgO, SDL_Texture* bkgP)
    : GUIObject(a, disp, dims, parent)
{
    this->func = func;
    this->bkgNormal = bkgN;
    this->bkgOver = bkgO;
    this->bkgPressed = bkgP;
    willDestroyTextures = false;
}

GUIButton::GUIButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent,
                     void (*func) (void), SDL_Texture* bkgN, SDL_Texture* bkgO)
    : GUIObject(a, disp, dims, parent)
{
    this->func = func;
    this->bkgNormal = bkgN;
    this->bkgOver = bkgO;
    this->bkgPressed = bkgO;
    willDestroyTextures = false;
}

GUIButton::GUIButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent,
                     void (*func) (void), SDL_Texture* bkgN)
    : GUIObject(a, disp, dims, parent)
{
    this->func = func;
    this->bkgNormal = bkgN;
    this->bkgOver = bkgN;
    this->bkgPressed = bkgN;
    willDestroyTextures = false;
}

GUIButton::GUIButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent,
                     void (*func) (void))
    : GUIObject(a, disp, dims, parent)
{
    this->func = func;
    this->bkgNormal = NULL;
    this->bkgOver = NULL;
    this->bkgPressed = NULL;
    willDestroyTextures = false;
}

GUIButton::~GUIButton()
{
    if (willDestroyTextures)
    {
        if (bkgNormal != NULL) SDL_DestroyTexture(bkgNormal);
        if (bkgOver != NULL) SDL_DestroyTexture(bkgOver);
        if (bkgPressed != NULL) SDL_DestroyTexture(bkgPressed);
    }
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
