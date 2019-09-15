#include "GUITexture.h"
#include "Global.h"
#include "ResourceLoader.h"

GUITexture::GUITexture(ANCHOR a, Coord disp, SDL_Texture* tex, Coord dims, GUIContainer* par)
    : GUIObject(a, disp, dims, par)
{
    texture = tex;
    willDestroyTexture = false;
}

GUITexture::GUITexture(ANCHOR a, Coord disp, SDL_Texture* tex, Coord dims, bool destroyTex, GUIContainer* par)
    : GUIObject(a, disp, dims, par)
{
    texture = tex;
    willDestroyTexture = destroyTex;
}

GUITexture::GUITexture(ANCHOR a, Coord disp, std::string tex, Coord dims, GUIContainer* par)
    : GUIObject(a, disp, dims, par)
{
    texture = loadTexture(tex);
    willDestroyTexture = true;
}

GUITexture::GUITexture(ANCHOR a, Coord disp, std::string str, GUIContainer* parent)
    : GUIObject(a, disp, {0, 0}, parent)
{
    texture = loadString(str, FONT_NORMAL, 50, {255, 255, 255, 255});
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    setBounds(disp, {w, h});
    willDestroyTexture = true;
}

GUITexture::~GUITexture()
{
    if (willDestroyTexture)
        SDL_DestroyTexture(texture);
}

bool GUITexture::mouseDown()
{
    return false;
}

bool GUITexture::mouseUp()
{
    return true;
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
