#include "Standard.h"
#include "GUIObject.h"

#include "GUIContainer.h"
#include "Global.h"

GUIObject::GUIObject(ANCHOR anchorPoint, Coord disp, Coord dims, GUIContainer* parent)
{
    anchor = anchorPoint;
    displacement = disp;
    pressed = false;

    startAlpha = 255;
    endAlpha = 255;
    currAlpha = 255;
    fadeCurrDuration = 0;
    fadeInitDuration = 0;

    this->parent = parent;
    setBounds(disp, dims);
}

GUIObject::~GUIObject()
{
    //dtor
}

void GUIObject::setBounds(Coord disp, Coord dims)
{
    displacement = disp;
	bounds.w = dims.x;
	bounds.h = dims.y;
    bounds.x = getXAnchor() + disp.x;
    bounds.y = getYAnchor() + disp.y;
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
        return parent->getBounds()->x + parent->getBounds()->w - bounds.w;
    case ANCHOR_NORTH:
    case ANCHOR_CENTER:
    case ANCHOR_SOUTH:
        return parent->getBounds()->x + parent->getBounds()->w/2 - bounds.w/2;
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
        return parent->getBounds()->y + parent->getBounds()->h/2 - bounds.h/2;
    case ANCHOR_SOUTHEAST:
    case ANCHOR_SOUTH:
    case ANCHOR_SOUTHWEST:
        return parent->getBounds()->y + parent->getBounds()->h - bounds.h;
    }

    return -1;
}

SDL_Rect* GUIObject::getBounds()
{
    return &bounds;
}

void GUIObject::recomputePosition()
{
    bounds.x = getXAnchor() + displacement.x;
    bounds.y = getYAnchor() + displacement.y;
}

bool GUIObject::isMouseOver()
{
    if (_mousePos.x >= bounds.x &&
        _mousePos.x < bounds.x + bounds.w &&
        _mousePos.y >= bounds.y &&
        _mousePos.y < bounds.y + bounds.h)
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
    if (_debug >= DEBUG_NORMAL)
    {
        if (isMouseOver())
        {
            if (pressed)
            {
                SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
            }
        }
        else
        {
            if (pressed)
            {
                SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
            }
        }
        SDL_RenderDrawRect(_renderer, &bounds);
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
            setTransparency(startAlpha - (int)(pctDone*(startAlpha - endAlpha)) - 1);
        }
        else // if alpha is increasing
        {
            setTransparency((int)(pctDone*(endAlpha - startAlpha)) + 1);
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
