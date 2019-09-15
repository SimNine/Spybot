#include <stdlib.h>
#include <SDL.h>

#include "MainScreenGlow.h"
#include "Global.h"

MainScreenGlow::MainScreenGlow(Coord p)
{
    xPos = p.x;
    yPos = p.y;
    xVel = 0;
    yVel = 0;
    xAcc = rand()/(float)RAND_MAX - 0.5;
    yAcc = rand()/(float)RAND_MAX - 0.5;

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
    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = 200;
    destRect.h = 200;

    SDL_RenderCopy(gRenderer, dataContainer->main_bkgsplotch, NULL, &destRect);
}

void MainScreenGlow::tick(int ms)
{
    // 2% chance of changing accelerations
    if ((rand() % 500) == 1)
    {
        xAcc = (rand()/RAND_MAX - 0.5)/1000.0;
        yAcc = (rand()/RAND_MAX - 0.5)/1000.0;
        aAcc = (rand()/RAND_MAX - 0.5)/1000.0;
    }

    // update physics
    xVel += xAcc*ms/100.0;
    yVel += yAcc*ms/100.0;
    aVel += aAcc*ms/100.0;

    xPos += xVel*ms;
    yPos += yVel*ms;
    aPos += aVel*ms;

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
