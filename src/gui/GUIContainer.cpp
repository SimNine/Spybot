#include <SDL.h>
#include <stdio.h>

#include "GUIContainer.h"
#include "LinkedList.h"
#include "Global.h"

GUIContainer::GUIContainer(Anch anchorPoint, int xRel, int yRel, int width, int height,
                           GUIContainer* parent, SDL_Texture* bkg)
    : GUIObject(anchorPoint, xRel, yRel, width, height, parent)
{
    bkgImg = bkg;
    contents = NULL;
}

GUIContainer::~GUIContainer()
{
    //dtor
}

SDL_Texture* GUIContainer::getBackground()
{
    return bkgImg;
}

void GUIContainer::setBackground(SDL_Texture* bkg)
{
    bkgImg = bkg;
}

LinkedList<GUIObject*>* GUIContainer::getContents()
{
    return contents;
}

// looks for and attempts to click an objct within this container
bool GUIContainer::mouseDown()
{
    bool ret = false;
    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        GUIObject* curr = currNode->getContents();
        if (curr->isVisible() && curr->isMouseOver())
        {
            ret = true;
            curr->mouseDown();
        }
        currNode = currNode->getNext();
    }
    return ret;
}

bool GUIContainer::mouseUp()
{
    bool ret = false;
    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        GUIObject* curr = currNode->getContents();
        if (curr->isVisible() && curr->isMouseOver())
        {
            ret = true;
            curr->mouseUp();
        }
        curr->setPressed(false);
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

    if (bkgImg != NULL)
    {
        // draw background image
        SDL_RenderCopy(gRenderer, bkgImg, NULL, &bounds);
    }
}

void GUIContainer::drawContents()
{
    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        if (currNode->getContents()->isVisible()) currNode->getContents()->draw();
        currNode = currNode->getNext();
    }
}

void GUIContainer::draw()
{
    drawBkg();
    drawContents();

    if (debug) drawBounds();
}

void GUIContainer::setTransparency(int a)
{
    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        currNode->getContents()->setTransparency(a);
        currNode = currNode->getNext();
    }
}

void GUIContainer::tick()
{
    LinkedList<GUIObject*>* currNode = contents;
    while (currNode != NULL)
    {
        currNode->getContents()->tick();
        currNode = currNode->getNext();
    }
}
