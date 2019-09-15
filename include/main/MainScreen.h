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
        void tick(int);

        void toggleOptions();
    protected:
    private:
        LinkedList<MainScreenGlow*>* glowList;

        GUIContainer* optionsContainer;
        GUIContainer* mainContainer;
};

#endif // MAINSCREEN_H
