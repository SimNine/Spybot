#include "GUIObject.h"
#include "Global.h"
#include <stdio.h>

GUIObject::GUIObject(ANCHOR anchorPoint, int xRel, int yRel, int width, int height, GUIContainer* parent)
{
    anchor = anchorPoint;
    xDisplacement = xRel;
    yDisplacement = yRel;
    pressed = false;

    startAlpha = 255;
    endAlpha = 255;
    currAlpha = 255;
    fadeCurrDuration = 0;
    fadeInitDuration = 0;

    this->parent = parent;
    setBounds(xRel, yRel, width, height);
}

GUIObject::~GUIObject()
{
    //dtor
}

void GUIObject::setBounds(int xRel, int yRel, int width, int height)
{
    xDisplacement = xRel;
    yDisplacement = yRel;
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
    case ANCHOR_NORTHEAST:
    case ANCHOR_EAST:
    case ANCHOR_SOUTHEAST:
        return parent->getBounds()->x + parent->getBounds()->w;
    case ANCHOR_NORTH:
    case ANCHOR_CENTER:
    case ANCHOR_SOUTH:
        return parent->getBounds()->x + parent->getBounds()->w/2;
    case ANCHOR_NORTHWEST:
    case ANCHOR_WEST:
    case ANCHOR_SOUTHWEST:
        return parent->getBounds()->x;
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
    case ANCHOR_NORTHWEST:
    case ANCHOR_NORTH:
    case ANCHOR_NORTHEAST:
        return parent->getBounds()->y;
    case ANCHOR_WEST:
    case ANCHOR_CENTER:
    case ANCHOR_EAST:
        return parent->getBounds()->y + parent->getBounds()->h/2;
    case ANCHOR_SOUTHEAST:
    case ANCHOR_SOUTH:
    case ANCHOR_SOUTHWEST:
        return parent->getBounds()->y + parent->getBounds()->h;
    }

    return -1;
}

SDL_Rect* GUIObject::getBounds()
{
    return &bounds;
}

void GUIObject::recomputePosition()
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
    return (currAlpha > 0);
}

void GUIObject::setPressed(bool b)
{
    pressed = b;
}

void GUIObject::drawBounds()
{
    if (debug >= DEBUG_NORMAL)
    {
        if (isMouseOver())
        {
            if (pressed)
            {
                SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
            }
        }
        else
        {
            if (pressed)
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            }
        }
        SDL_RenderDrawRect(gRenderer, &bounds);
    }
}

void GUIObject::fade(int endAlpha, int fadeDuration)
{
    this->startAlpha = currAlpha;
    this->endAlpha = endAlpha;
    this->fadeInitDuration = fadeDuration;
    this->fadeCurrDuration = 0;
}

int GUIObject::getTransparency()
{
    return currAlpha;
}

void GUIObject::fadeStep(int ms)
{
    if (fadeCurrDuration != fadeInitDuration) // if fading is occurring
    {
        double pctDone = (double)fadeCurrDuration/fadeInitDuration;

        if (currAlpha > endAlpha) // if alpha is decreasing
        {
            setTransparency(startAlpha - pctDone*(startAlpha - endAlpha) - 1);
        }
        else // if alpha is increasing
        {
            setTransparency(pctDone*(endAlpha - startAlpha) + 1);
        }

        // increment the fade duration
        fadeCurrDuration += ms;

        // if the fade duration has ended
        if (fadeCurrDuration >= fadeInitDuration)
        {
            fadeCurrDuration = 0;
            fadeInitDuration = 0;

            // guarantee visibility or invisibility
            if (endAlpha == 255) setTransparency(255);
            else if (endAlpha == 0) setTransparency(0);
        }
    }
}
