#include "GUIObject.h"
#include "Global.h"
#include <stdio.h>

GUIObject::GUIObject(Anch anchorPoint, int xRel, int yRel, int width, int height, GUIContainer* parent)
{
    anchor = anchorPoint;
    xDisplacement = xRel;
    yDisplacement = yRel;
    this->parent = parent;
    pressed = false;
    visible = true;
    setBounds(xRel, yRel, width, height);
}

GUIObject::~GUIObject()
{
    //dtor
}

void GUIObject::setBounds(int xRel, int yRel, int width, int height)
{
    bounds.x = getXAnchor() + xRel;
    bounds.y = getYAnchor() + yRel;
    bounds.w = width;
    bounds.h = height;
}

int GUIObject::getXAnchor()
{
    if (parent == NULL)
    {
        return 0;
    }

    switch (anchor)
    {
    case ANCHOR_TOP_LEFT:
        return parent->getBounds()->x;
    case ANCHOR_TOP_RIGHT:
        return parent->getBounds()->x + parent->getBounds()->w;
    case ANCHOR_BOTTOM_LEFT:
        return parent->getBounds()->x;
    case ANCHOR_BOTTOM_RIGHT:
        return parent->getBounds()->x + parent->getBounds()->w;
    case ANCHOR_CENTER:
        return parent->getBounds()->x + parent->getBounds()->w/2;
    }

    return -1;
}

int GUIObject::getYAnchor()
{
    if (parent == NULL)
    {
        return 0;
    }

    switch (anchor)
    {
    case ANCHOR_TOP_LEFT:
        return parent->getBounds()->y;
    case ANCHOR_TOP_RIGHT:
        return parent->getBounds()->y;
    case ANCHOR_BOTTOM_LEFT:
        return parent->getBounds()->y + parent->getBounds()->h;
    case ANCHOR_BOTTOM_RIGHT:
        return parent->getBounds()->y + parent->getBounds()->h;
    case ANCHOR_CENTER:
        return parent->getBounds()->y + parent->getBounds()->h/2;
    }

    return -1;
}

SDL_Rect* GUIObject::getBounds()
{
    return &bounds;
}

void GUIObject::resetBounds()
{
    bounds.x = getXAnchor() + xDisplacement;
    bounds.y = getYAnchor() + yDisplacement;
}

bool GUIObject::isMouseOver()
{
    if (mousePosX >= bounds.x &&
            mousePosX < bounds.x + bounds.w &&
            mousePosY >= bounds.y &&
            mousePosY < bounds.y + bounds.h)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool GUIObject::isVisible()
{
    return visible;
}

void GUIObject::setVisible(bool b)
{
    visible = b;
}

void GUIObject::setPressed(bool b)
{
    pressed = b;
}

void GUIObject::drawBounds()
{
    if (debug)
    {
        if (isMouseOver())
        {
            if (pressed)
            {
                SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 0);
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
            }
        }
        else
        {
            if (pressed)
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0);
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
            }
        }
        SDL_RenderDrawRect(gRenderer, &bounds);
    }
}
