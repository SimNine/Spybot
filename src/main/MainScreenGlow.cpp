#include <stdlib.h>
#include <SDL.h>

#include "MainScreenGlow.h"
#include "Global.h"

MainScreenGlow::MainScreenGlow(int x, int y)
{
    xPos = x;
    yPos = y;
    xVel = 0;
    yVel = 0;
    xAcc = rand()/(float)RAND_MAX - 0.5;
    yAcc = rand()/(float)RAND_MAX - 0.5;

    aPos = 1;
    aVel = 0;
    aAcc = rand()/(float)RAND_MAX;

    //printf("%4f,%4f,%4f\n", xAcc, yAcc, aAcc);
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

void MainScreenGlow::tick()
{
    // 2% chance of changing accelerations
    if ((rand() % 500) == 1)
    {
        xAcc = rand()/RAND_MAX - 0.5;
        yAcc = rand()/RAND_MAX - 0.5;
        aAcc = rand()/RAND_MAX - 0.5;
    }

    // update physics
    xVel += xAcc;
    yVel += yAcc;
    aVel += aAcc;

    xPos += xVel;
    yPos += yVel;
    aPos += aVel;

    if (aPos < 0)
    {
        aPos = 0;
        aVel = 0;
        aAcc = 0;
    }
    else if (aPos > 255)
    {
        aVel = -aVel;
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

int MainScreenGlow::getXPos()
{
    return xPos;
}

int MainScreenGlow::getYPos()
{
    return yPos;
}
