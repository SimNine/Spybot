#ifndef PROGRAMINVENTORYDISPLAY_H
#define PROGRAMINVENTORYDISPLAY_H

#include <GUIContainer.h>


class ProgramInventoryDisplay : public GUIContainer
{
    public:
        ProgramInventoryDisplay(ANCHOR, Coord, Coord, GUIContainer*, int*);
        virtual ~ProgramInventoryDisplay();

        void resetBounds();
        void draw();

        void updateContents();
    protected:
    private:
        int* progList;

        int col;
};

#endif // PROGRAMINVENTORYDISPLAY_H
