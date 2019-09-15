#ifndef MAINSCREENGLOW_H
#define MAINSCREENGLOW_H

#include "Coord.h"

class MainScreenGlow
{
    public:
        MainScreenGlow(Coord);
        virtual ~MainScreenGlow();
        void draw();
        void tick(int);

        Coord getPos();
        bool getTransparent();
    protected:
    private:
        float xPos, yPos;
        float xVel, yVel;
        float xAcc, yAcc;

        float aPos;
        float aVel;
        float aAcc;
};

#endif // MAINSCREENGLOW_H
