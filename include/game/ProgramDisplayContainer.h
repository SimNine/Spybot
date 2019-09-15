#ifndef PROGRAMDISPLAYCONTAINER_H
#define PROGRAMDISPLAYCONTAINER_H

#include <GUIObject.h>

#include "GUITexture.h"
#include "LinkedList.h"

class ProgramDisplayContainer : public GUIObject
{
    public:
        ProgramDisplayContainer(ANCHOR, Coord, Coord, GUIContainer*);
        virtual ~ProgramDisplayContainer();

        void draw();
        void setTransparency(int);
        bool mouseDown();
        bool mouseUp();
        void tick(int);
        void resetBounds();
    protected:
    private:
        int currAlpha;
};

#endif // PROGRAMDISPLAYCONTAINER_H
