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
    SDL_SetTextureAlphaMod(texture, a);
}

bool GUITexture::mouseUp()
{
    return true;
}

void GUITexture::tick()
{
    return;
}
