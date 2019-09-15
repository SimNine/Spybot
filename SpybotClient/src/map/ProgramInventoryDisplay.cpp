#include "Standard.h"
#include "ProgramInventoryDisplay.h"

#include "ResourceLoader.h"
#include "GUITexture.h"
#include "Global.h"
#include "MiscUtil.h"
#include "ProgramInventoryButton.h"

ProgramInventoryDisplay::ProgramInventoryDisplay(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent)
    : GUIContainer(a, disp, dims, parent, _color_bkg_standard)
{
	resetBounds();
}

ProgramInventoryDisplay::~ProgramInventoryDisplay()
{
    //dtor
}

void ProgramInventoryDisplay::updateContents()
{
    // clear contents
    while (contents->getLength() > 0)
    {
        GUIObject* curr = contents->removeFirst();
        delete curr;
    }

    // create new contents
    int w, h;
	GUITexture* label = new GUITexture(ANCHOR_NORTHEAST, { -20, 20 }, "My Programs", 40, this);
	addObject(label);

    int col = 0;
    int row = 0;
    for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
    {
        if (i == PROGRAM_NONE || i == PROGRAM_CUSTOM) continue;

        if (_progListCurrent[i] != 0)
        {
            // create the counter
            std::string str = "x" + to_string(_progListCurrent[i]);
            SDL_Texture* tex = loadString(str, FONT_BOLD, 24, {255, 255, 255, 255});
            SDL_QueryTexture(tex, NULL, NULL, &w, &h);
            GUITexture* progTex = new GUITexture(ANCHOR_NORTHWEST, {100*col + 60, 100 + row*h}, tex, {w, h}, true, this);
            addObject(progTex);

            ProgramInventoryButton* progButton = new ProgramInventoryButton(ANCHOR_NORTHWEST, {100*col + 20, 100 + row*h}, (PROGRAM)i, this);
            addObject(progButton);

            row++;
            if (100 + (row+3)*h > _SCREEN_HEIGHT)
            {
                col++;
                row = 0;
            }
        }
    }

    setBounds({-20, 20}, {100*(col+1) + 40, _SCREEN_HEIGHT - 40});
}

void ProgramInventoryDisplay::resetBounds()
{
    updateContents();
    GUIContainer::resetBounds();
}
