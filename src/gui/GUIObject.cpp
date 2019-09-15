#include "GUIObject.h"
#include "Global.h"
#include <stdio.h>

GUIObject::GUIObject(Anch anchorPoint, int xRel, int yRel, int width, int height, GUIContainer* parent)
{
    anchor = anchorPoint;
    xDisplacement = xRel;
    yDisplacement = yRel;
    this->parent = parent;
    bounds = new SDL_Rect();
    setBounds(xRel, yRel, width, height);
}

GUIObject::~GUIObject()
{
    //dtor
}

void GUIObject::setBounds(int xRel, int yRel, int width, int height)
{
    bounds->x = getXAnchor() + xRel;
    bounds->y = getYAnchor() + yRel;
    bounds->w = width;
    bounds->h = height;
}

int GUIObject::getXAnchor()
{
    if (parent == NULL)
    {
        return 0;
    }

    switch (anchor)
    {
    case TOP_LEFT:
        return parent->getBounds()->x;
    case TOP_RIGHT:
        return parent->getBounds()->x + parent->getBounds()->w;
    case BOTTOM_LEFT:
        return parent->getBounds()->x;
    case BOTTOM_RIGHT:
        return parent->getBounds()->x + parent->getBounds()->w;
    case CENTER:
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
    case TOP_LEFT:
        return parent->getBounds()->y;
    case TOP_RIGHT:
        return parent->getBounds()->y;
    case BOTTOM_LEFT:
        return parent->getBounds()->y + parent->getBounds()->h;
    case BOTTOM_RIGHT:
        return parent->getBounds()->y + parent->getBounds()->h;
    case CENTER:
        return parent->getBounds()->y + parent->getBounds()->h/2;
    }

    return -1;
}

SDL_Rect* GUIObject::getBounds()
{
    return bounds;
}

void GUIObject::resetBounds()
{
    bounds->x = getXAnchor() + xDisplacement;
    bounds->y = getYAnchor() + yDisplacement;
}

bool GUIObject::isMouseOver()
{
    if (mousePosX >= bounds->x &&
        mousePosX < bounds->x + bounds->w &&
        mousePosY >= bounds->y &&
        mousePosY < bounds->y + bounds->h)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool GUIObject::getVisible()
{
    return visible;
}

void GUIObject::setVisible(bool b)
{
    visible = b;
}
