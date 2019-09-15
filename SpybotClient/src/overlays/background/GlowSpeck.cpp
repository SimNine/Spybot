#include "Standard.h"
#include "GlowSpeck.h"

#include "Data.h"
#include "Global.h"

GlowSpeck::GlowSpeck() {
	radius_ = (rand() % 300);

	rCol_ = rand() % 255;
	gCol_ = rand() % 255;
	bCol_ = rand() % 255;

	xPos_ = (float)(rand() % (_screenWidth + radius_ * 2) - radius_);
	yPos_ = (float)(rand() % (_screenHeight + radius_ * 2) - radius_);
	xVel_ = 0;
	yVel_ = 0;
	xAcc_ = ((float)rand() / (float)RAND_MAX - (float)0.5) / (float)50;
	yAcc_ = ((float)rand() / (float)RAND_MAX - (float)0.5) / (float)50;

	aPos_ = 0;
	aVel_ = 0;
	aAcc_ = (rand() / (float)RAND_MAX) / (float)5;
}

GlowSpeck::~GlowSpeck() {
	// dtor
}

void GlowSpeck::draw() {
	//SDL_SetTextureColorMod(_main_bkgsplotch2, rCol, gCol, bCol);
	SDL_SetTextureAlphaMod(_main_bkgsplotch, (Uint8)aPos_);

	SDL_Rect destRect;
	destRect.x = (int)xPos_;
	destRect.y = (int)yPos_;
	destRect.w = radius_;
	destRect.h = radius_;

	SDL_RenderCopy(_renderer, _main_bkgsplotch, NULL, &destRect);
}

void GlowSpeck::tick(int ms) {
	// update physics
	xVel_ += (float)xAcc_*(float)ms / (float)100.0;
	yVel_ += (float)yAcc_*(float)ms / (float)100.0;
	aVel_ += (float)aAcc_*(float)ms / (float)100.0;

	xPos_ += (float)xVel_*(float)ms;
	yPos_ += (float)yVel_*(float)ms;
	aPos_ += (float)aVel_*(float)ms;

	if (aPos_ <= 0) {
		aPos_ = 0;
		aVel_ = 0;
		aAcc_ = 0;
	} else if (aPos_ > 255) {
		aAcc_ = -aAcc_;
		aVel_ = 0;
		aPos_ = 255;
	}
}

bool GlowSpeck::getTransparent() {
	return (aPos_ <= 0);
}

Coord GlowSpeck::getPos() {
	return { (int)xPos_, (int)yPos_ };
}

int GlowSpeck::getRadius() {
	return radius_;
}