#include <SDL.h>
#include <stdio.h>

#include "GUIContainer.h"
#include "LinkedList.h"
#include "Global.h"

GUIContainer::GUIContainer(ANCHOR anchorPoint, int xRel, int yRel, int width, int height,
                           GUIContainer* parent, SDL_Texture* bkg)
    : GUIObject(anchorPoint, xRel, yRel, width, height, parent)
{
    bkgImg = bkg;
    contents = new LinkedList<GUIObject*>();
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
    Iterator<GUIObject*> it = contents->getIterator();
    while (it.hasNext())
    {
        GUIObject* curr = it.next();
        if (curr->isVisible() && curr->isMouseOver())
        {
            ret = true;
            curr->mouseDown();
        }
    }
    return ret;
}

bool GUIContainer::mouseUp()
{
    bool ret = false;
    Iterator<GUIObject*> it = contents->getIterator();
    while (it.hasNext())
    {
        GUIObject* curr = it.next();
        if (curr->isVisible() && curr->isMouseOver())
        {
            ret = true;
            curr->mouseUp();
        }
        curr->setPressed(false);
    }
    return ret;
}

void GUIContainer::setPressed(bool p)
{
    Iterator<GUIObject*> it = contents->getIterator();
    while (it.hasNext())
    {
        it.next()->setPressed(p);
    }
}

void GUIContainer::resetBounds()
{
    recomputePosition();

    if (parent == NULL)
    {
        setBounds(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    contents->forEach([](GUIObject* g){g->resetBounds();});
}

void GUIContainer::addObject(GUIObject* obj)
{
    contents->addLast(obj);
}

void GUIContainer::addAllObjects(LinkedList<GUIObject*>* objList)
{
    contents->addAllLast(objList);
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
    contents->forEach([](GUIObject* g){if (g->isVisible()) g->draw();});
}

void GUIContainer::draw()
{
    drawBkg();
    drawContents();

    if (debug)
    {
        drawBounds();
    }
}

void GUIContainer::setTransparency(int a)
{
    if (a > 255) currAlpha = 255;
    else if (a < 0) currAlpha = 0;
    else currAlpha = a;

    Iterator<GUIObject*> it = contents->getIterator();
    while (it.hasNext())
    {
        it.next()->setTransparency(currAlpha);
    }
}

void GUIContainer::tick(int ms)
{
    fadeStep(ms);

    Iterator<GUIObject*> it = contents->getIterator();
    while (it.hasNext())
    {
        it.next()->tick(ms);
    }
}
