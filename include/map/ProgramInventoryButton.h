#ifndef PROGRAMINVENTORYBUTTON_H
#define PROGRAMINVENTORYBUTTON_H

#include <GUIObject.h>

class ProgramInventoryButton : public GUIObject
{
    public:
        ProgramInventoryButton(ANCHOR, Coord, int*, PROGRAM, GUIContainer*);
        virtual ~ProgramInventoryButton();

        void setTransparency(int);
        bool mouseDown();
        bool mouseUp();
        void draw();
        void tick(int);
        void resetBounds();
    protected:
    private:
        SDL_Texture* icon;
        int* progList;
        PROGRAM progType;
};

#endif // PROGRAMINVENTORYBUTTON_H
