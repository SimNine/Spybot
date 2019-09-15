#include "GUIButton.h"
#include "Global.h"
#include <stdio.h>

GUIButton::GUIButton(Anch a, int xDisp, int yDisp, int width, int height, GUIContainer* parent,
                     void (*func) (void), SDL_Texture* bkgN, SDL_Texture* bkgO, SDL_Texture* bkgP)
    : GUIObject(a, xDisp, yDisp, width, height, parent)
{
    this->func = func;
    this->bkgNormal = bkgN;
    this->bkgOver = bkgO;
    this->bkgPressed = bkgP;
}

GUIButton::~GUIButton()
{
    //dtor
}

void GUIButton::draw()
{
    if (mousePressed && isMouseOver() && bkgPressed != NULL)
    {
        SDL_RenderCopy(gRenderer, bkgPressed, NULL, bounds);
    }
    else if (!mousePressed && isMouseOver() && bkgOver != NULL)
    {
        SDL_RenderCopy(gRenderer, bkgOver, NULL, bounds);
    }
    else if (bkgNormal != NULL)
    {
        SDL_RenderCopy(gRenderer, bkgNormal, NULL, bounds);
    }

    if (debug)
    {
        if (!isMouseOver())
        {
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
        }
        else
        {
            SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
        }
        SDL_RenderDrawRect(gRenderer, bounds);
    }
}

bool GUIButton::click()
{
    func();
    return true;
}
