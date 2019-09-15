#ifndef PROGRAMDISPLAYCONTAINER_H
#define PROGRAMDISPLAYCONTAINER_H

#include <GUIObject.h>


class ProgramDisplayContainer : public GUIObject
{
    public:
        ProgramDisplayContainer(ANCHOR, int, int, int, int, GUIContainer*);
        virtual ~ProgramDisplayContainer();

        void draw();
        void setTransparency(int);
        bool mouseDown();
        bool mouseUp();
        void tick();
    protected:
    private:
        int transparency;
};

#endif // PROGRAMDISPLAYCONTAINER_H
