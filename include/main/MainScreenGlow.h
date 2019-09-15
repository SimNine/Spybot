#ifndef MAINSCREENGLOW_H
#define MAINSCREENGLOW_H


class MainScreenGlow
{
    public:
        MainScreenGlow(int, int);
        virtual ~MainScreenGlow();
        void draw();
        void tick(int);

        int getXPos();
        int getYPos();
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
