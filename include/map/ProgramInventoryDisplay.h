#ifndef PROGRAMINVENTORYDISPLAY_H
#define PROGRAMINVENTORYDISPLAY_H

#include <GUIContainer.h>


class ProgramInventoryDisplay : public GUIContainer
{
    public:
        ProgramInventoryDisplay(ANCHOR, Coord, Coord, GUIContainer*);
        virtual ~ProgramInventoryDisplay();

        void resetBounds();
        void updateContents();
    protected:
    private:
        int col;
};

#endif // PROGRAMINVENTORYDISPLAY_H
