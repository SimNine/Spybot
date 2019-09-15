#include <SDL.h>
#include <stdio.h>

#include "GUIContainer.h"
#include "LinkedList.h"
#include "Global.h"

GUIContainer::GUIContainer(Anch anchorPoint, int xRel, int yRel, int width, int height,
                           GUIContainer* parent, SDL_Texture* bkg)
    : GUIObject(anchorPoint, xRel, yRel, width, height, parent)
{
    background = bkg;
    contents = NULL;
}

GUIContainer::~GUIContainer()
{
    //dtor
}

SDL_Texture* GUIContainer::getBackground()
{
    return background;
}

void GUIContainer::setBackground(SDL_Texture* bkg)
{
    background = bkg;
}

LinkedList<GUIObject*>* GUIContainer::getContents()
{
    return contents;
}

// looks for and attempts to click an objct within this container
bool GUIContainer::click()
{
    bool ret = false;
    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        GUIObject* curr = currNode->getContents();
        if (curr->isMouseOver())
        {
            ret = true;
            curr->click();
        }
        currNode = currNode->getNext();
    }
    return ret;
}

void GUIContainer::resetBounds()
{
    GUIObject::resetBounds();

    if (parent == NULL)
    {
        setBounds(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        currNode->getContents()->resetBounds();
        currNode = currNode->getNext();
    }
}

void GUIContainer::addObject(GUIObject* obj)
{
    if (contents == NULL)
    {
        contents = new LinkedList<GUIObject*>(obj);
    }
    else
    {
        LL_addObject(contents, obj);
    }
}

void GUIContainer::addAllObjects(LinkedList<GUIObject*>* objList)
{
    if (contents == NULL)
    {
        contents = objList;
    }
    else
    {
        LL_addAllObjects(contents, objList);
    }
}

void GUIContainer::drawBkg()
{
    // if this container has no parent (and is therefore an overlay)
    if (parent == NULL)
    {
        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
    }

    // draw background image
    SDL_RenderCopy(gRenderer, background, NULL, bounds);
}

void GUIContainer::drawContents()
{
    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        currNode->getContents()->draw();
        currNode = currNode->getNext();
    }
}

void GUIContainer::draw()
{
    drawBkg();
    drawContents();

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
