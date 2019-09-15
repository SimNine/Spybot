#ifndef PROGRAMDISPLAYCONTAINER_H
#define PROGRAMDISPLAYCONTAINER_H

#include "Standard.h"
#include "GUIContainer.h"

class ProgramDisplayContainer : public GUIContainer
{
    public:
        ProgramDisplayContainer(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent);
        virtual ~ProgramDisplayContainer();

        void draw();
    protected:
    private:
};

#endif // PROGRAMDISPLAYCONTAINER_H
