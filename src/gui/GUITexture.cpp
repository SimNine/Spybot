#include "GUITexture.h"
#include "Global.h"
#include "ResourceLoader.h"

GUITexture::GUITexture(ANCHOR a, int xDisp, int yDisp, SDL_Texture* tex, int w, int h, GUIContainer* par)
    : GUIObject(a, xDisp, yDisp, w, h, par)
{
    texture = tex;
    willDestroyTexture = false;
}

GUITexture::GUITexture(ANCHOR a, int xDisp, int yDisp, SDL_Texture* tex, int w, int h, bool destroyTex, GUIContainer* par)
    : GUIObject(a, xDisp, yDisp, w, h, par)
{
    texture = tex;
    willDestroyTexture = destroyTex;
}

GUITexture::GUITexture(ANCHOR a, int xDisp, int yDisp, std::string str, GUIContainer* parent)
    : GUIObject(a, xDisp, yDisp, 0, 0, parent)
{
    texture = loadString(str, FONT_NORMAL, 50, {255, 255, 255, 255});
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    setBounds(xDisp, yDisp, w, h);
    willDestroyTexture = true;
}

GUITexture::~GUITexture()
{
    if (willDestroyTexture) SDL_DestroyTexture(texture);
}

bool GUITexture::mouseDown()
{
    return false;
}

void GUITexture::draw()
{
    SDL_RenderCopy(gRenderer, texture, NULL, &bounds);

    if (debug >= DEBUG_NORMAL) drawBounds();
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
