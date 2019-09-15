#include "Standard.h"
#include "MainScreenGlow.h"

#include "DataContainer.h"
#include "Global.h"

MainScreenGlow::MainScreenGlow(Coord p)
{
    xPos = (float)p.x;
    yPos = (float)p.y;
    xVel = 0;
    yVel = 0;
    xAcc = (float)rand()/(float)RAND_MAX - (float)0.5;
    yAcc = (float)rand()/(float)RAND_MAX - (float)0.5;

    aPos = 1;
    aVel = 0;
    aAcc = rand()/(float)RAND_MAX;
}

MainScreenGlow::~MainScreenGlow()
{

}

void MainScreenGlow::draw()
{
    SDL_SetTextureAlphaMod(dataContainer->main_bkgsplotch, (Uint8)aPos);

    SDL_Rect destRect;
    destRect.x = (int)xPos;
    destRect.y = (int)yPos;
    destRect.w = 200;
    destRect.h = 200;

    SDL_RenderCopy(gRenderer, dataContainer->main_bkgsplotch, NULL, &destRect);
}

void MainScreenGlow::tick(int ms)
{
    // 2% chance of changing accelerations
    if ((rand() % 500) == 1)
    {
        xAcc = ((float)rand()/(float)RAND_MAX - (float)0.5)/(float)1000.0;
        yAcc = ((float)rand()/(float)RAND_MAX - (float)0.5)/(float)1000.0;
        aAcc = ((float)rand()/(float)RAND_MAX - (float)0.5)/(float)1000.0;
    }

    // update physics
    xVel += (float)xAcc*(float)ms/(float)100.0;
    yVel += (float)yAcc*(float)ms/(float)100.0;
    aVel += (float)aAcc*(float)ms/(float)100.0;

    xPos += (float)xVel*(float)ms;
    yPos += (float)yVel*(float)ms;
    aPos += (float)aVel*(float)ms;

    if (aPos < 0)
    {
        aPos = 0;
        aVel = 0;
        aAcc = 0;
    }
    else if (aPos > 255)
    {
        aVel = -aVel;
        aPos = 255;
    }
}

bool MainScreenGlow::getTransparent()
{
    if (aPos < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Coord MainScreenGlow::getPos()
{
    return {(int)xPos, (int)yPos};
}
