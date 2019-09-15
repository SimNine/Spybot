#ifndef PROGRAMDISPLAYCONTAINER_H
#define PROGRAMDISPLAYCONTAINER_H

#include "GUITexture.h"
#include "LinkedList.h"
#include "GUIContainer.h"

class ProgramDisplayContainer : public GUIContainer
{
    public:
        ProgramDisplayContainer(ANCHOR, Coord, Coord, GUIContainer*);
        virtual ~ProgramDisplayContainer();

        void draw();
    protected:
    private:
};

#endif // PROGRAMDISPLAYCONTAINER_H
