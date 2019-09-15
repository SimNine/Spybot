#ifndef MAINSCREENGLOW_H
#define MAINSCREENGLOW_H

#include "Standard.h"

class MainScreenGlow
{
    public:
        MainScreenGlow();
        virtual ~MainScreenGlow();
        void draw();
        void tick(int);

        Coord getPos();
		int getRadius();
        bool getTransparent();
    protected:
    private:
		int radius_;

		Uint8 rCol;
		Uint8 gCol;
		Uint8 bCol;

        float xPos, yPos;
        float xVel, yVel;
        float xAcc, yAcc;

        float aPos;
        float aVel;
        float aAcc;
};

#endif // MAINSCREENGLOW_H
