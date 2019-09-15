#include "GUITexture.h"
#include "Global.h"

GUITexture::GUITexture(ANCHOR a, int xDisp, int yDisp, SDL_Texture* tex, int w, int h, GUIContainer* par)
    : GUIObject(a, xDisp, yDisp, w, h, par)
{
    texture = tex;
}

GUITexture::~GUITexture()
{
    //dtor
}

bool GUITexture::mouseDown()
{
    return false;
}

void GUITexture::draw()
{
    SDL_RenderCopy(gRenderer, texture, NULL, &bounds);

    if (debug) drawBounds();
}

void GUITexture::setTransparency(int a)
{
    if (a > 255) currAlpha = 255;
    else if (a < 0) currAlpha = 0;
    else currAlpha = a;

    SDL_SetTextureAlphaMod(texture, currAlpha);
}

bool GUITexture::mouseUp()
{
    return true;
}

void GUITexture::tick(int ms)
{
    fadeStep(ms);
    return;
}

SDL_Texture* GUITexture::swapTexture(SDL_Texture* t)
{
    SDL_Texture* temp = texture;
    texture = t;
    return temp;
}

void GUITexture::resetBounds()
{
    recomputePosition();
}
