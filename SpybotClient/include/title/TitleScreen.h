#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "Standard.h"
#include "GUIContainer.h"

class TitleScreen : public GUIContainer
{
    public:
        TitleScreen();
        virtual ~TitleScreen();

        void tick(int);
        void draw();
        bool mouseDown();
    protected:
    private:
        //LinkedList<GUITexture*>* sprites;

        int tickCount;

        int timingCompany;
        int timingTitle;
        int timingSubtitle;
        int spanTiming;

        void advance();
        void endTitles();
};

#endif // TITLESCREEN_H
