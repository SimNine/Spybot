#include "ProgramInventoryDisplay.h"
#include "ProgramInventoryButton.h"
#include "ResourceLoader.h"
#include "GUITexture.h"
#include "MiscUtil.h"
#include "Global.h"

ProgramInventoryDisplay::ProgramInventoryDisplay(ANCHOR a, int xDisp, int yDisp, int w, int h, GUIContainer* parent, int* progList)
    : GUIContainer(a, 0, 0, 0, 0, parent, NULL)
{
    this->progList = progList;

    updateContents();
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
    SDL_Texture* moves = loadString("PROGRAMS:", FONT_BOLD, 35, {255, 255, 255, 255});
    SDL_QueryTexture(moves, NULL, NULL, &w, &h);
    GUITexture* moveTex = new GUITexture(ANCHOR_NORTHEAST, -w - 20, 20, moves, w, h, true, this);
    addObject(moveTex);

    col = 1;
    int row = 0;
    for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
    {
        if (i == PROGRAM_NONE || i == PROGRAM_CUSTOM) continue;

        if (progList[i] != 0)
        {
            // create the counter
            std::string str = "x" + to_string(progList[i]);
            SDL_Texture* tex = loadString(str, FONT_BOLD, 24, {255, 255, 255, 255});
            SDL_QueryTexture(tex, NULL, NULL, &w, &h);
            GUITexture* progTex = new GUITexture(ANCHOR_NORTHEAST, (-100)*col + 40, 100 + row*h, tex, w, h, true, this);
            addObject(progTex);

            ProgramInventoryButton* progButton = new ProgramInventoryButton(ANCHOR_NORTHEAST, (-100)*col, 100 + row*h,
                                                                            progList, (PROGRAM)i, this);
            addObject(progButton);

            row++;
            if (100 + (row+3)*h > SCREEN_HEIGHT)
            {
                col++;
                row = 0;
            }
        }
    }

    setBounds(-40 - 100*col, 20, 100*col + 20, SCREEN_HEIGHT - 40);
}

void ProgramInventoryDisplay::resetBounds()
{
    updateContents();
    GUIContainer::resetBounds();
}

void ProgramInventoryDisplay::draw()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, currAlpha);
    SDL_RenderFillRect(gRenderer, &bounds);

    GUIContainer::draw();
}