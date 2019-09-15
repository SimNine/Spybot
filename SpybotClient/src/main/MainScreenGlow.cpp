#include "Standard.h"
#include "MainScreenGlow.h"

#include "Data.h"
#include "Global.h"

MainScreenGlow::MainScreenGlow()
{
	radius_ = (rand() % 300);

	rCol = rand() % 255;
	gCol = rand() % 255;
	bCol = rand() % 255;

    xPos = (float)(rand() % (_SCREEN_WIDTH + radius_*2) - radius_);
    yPos = (float)(rand() % (_SCREEN_HEIGHT + radius_*2) - radius_);
    xVel = 0;
    yVel = 0;
    xAcc = ((float)rand()/(float)RAND_MAX - (float)0.5)/(float)50;
    yAcc = ((float)rand()/(float)RAND_MAX - (float)0.5)/(float)50;

    aPos = 0;
    aVel = 0;
    aAcc = (rand()/(float)RAND_MAX)/(float)5;
}

MainScreenGlow::~MainScreenGlow()
{

}

void MainScreenGlow::draw()
{
	//SDL_SetTextureColorMod(_main_bkgsplotch2, rCol, gCol, bCol);
    SDL_SetTextureAlphaMod(_main_bkgsplotch, (Uint8)aPos);

    SDL_Rect destRect;
    destRect.x = (int)xPos;
    destRect.y = (int)yPos;
    destRect.w = radius_;
    destRect.h = radius_;

    SDL_RenderCopy(_renderer, _main_bkgsplotch, NULL, &destRect);
}

void MainScreenGlow::tick(int ms)
{
    // update physics
    xVel += (float)xAcc*(float)ms/(float)100.0;
    yVel += (float)yAcc*(float)ms/(float)100.0;
    aVel += (float)aAcc*(float)ms/(float)100.0;

    xPos += (float)xVel*(float)ms;
    yPos += (float)yVel*(float)ms;
    aPos += (float)aVel*(float)ms;

	if (aPos <= 0)
	{
		aPos = 0;
		aVel = 0;
		aAcc = 0;
	}
	else if (aPos > 255)
	{
		aAcc = -aAcc;
		aVel = 0;
		aPos = 255;
	}
}

bool MainScreenGlow::getTransparent()
{
	return (aPos <= 0);
}

Coord MainScreenGlow::getPos()
{
    return {(int)xPos, (int)yPos};
}

int MainScreenGlow::getRadius()
{
	return radius_;
}