#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <GUIContainer.h>

#include "MainScreenGlow.h"

class MainScreen : public GUIContainer
{
    public:
        MainScreen();
        virtual ~MainScreen();
        void draw();
        void tick();
    protected:
    private:
        LinkedList<MainScreenGlow*>* glowList;
};

#endif // MAINSCREEN_H
