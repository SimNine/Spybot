#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "GUIContainer.h"

class TitleScreen : public GUIContainer
{
    public:
        TitleScreen();
        virtual ~TitleScreen();
        void tick();
        void draw();
        bool mouseDown();
    protected:
    private:
        int tickCount;

        int timingCompany;
        int timingTitle;
        int timingSubtitle;

        void advance();
        void endTitles();
};

#endif // TITLESCREEN_H
