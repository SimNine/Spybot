#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class MainScreenGlow;

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
        int textBkgDisplacement;

        LinkedList<MainScreenGlow*>* glowList;

        GUIContainer* optionsContainer;
        GUIContainer* mainContainer;
};

#endif // MAINSCREEN_H
