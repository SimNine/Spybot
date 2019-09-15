#pragma once

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;
class ProgramAction;

class ProgramDisplayActionButton : public GUIObject
{
    public:
        ProgramDisplayActionButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent, ProgramAction* action, int actionNum);
        virtual ~ProgramDisplayActionButton();

        void setTransparency(int);
        bool mouseDown();
        bool mouseUp();
        void draw();
        void tick(int);
        void resetBounds();
    protected:
    private:
		ProgramAction* action_;
		int actionNum_;
};