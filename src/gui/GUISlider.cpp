#include "GUISlider.h"
#include "Global.h"

GUISlider::GUISlider(Anch a, int xD, int yD, int w, int h, GUIContainer* p, void (*func) (float))
    : GUIObject(a, xD, yD, w, h, p)
{
    sliderBounds.x = bounds.x;
    sliderBounds.y = bounds.y;
    sliderBounds.w = 20;
    sliderBounds.h = bounds.h;
    this->func = func;
}

GUISlider::~GUISlider()
{
    //dtor
}

void GUISlider::setTransparency(int a)
{
    transparency = a;
}

bool GUISlider::mouseDown()
{
    pressed = true;
    return true;
}

bool GUISlider::mouseUp()
{
    pressed = false;
    return true;
}

void GUISlider::draw()
{
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
    SDL_RenderDrawLine(gRenderer,
                       bounds.x + sliderBounds.w/2,
                       bounds.y + bounds.h/2,
                       bounds.x + bounds.w - sliderBounds.w/2,
                       bounds.y + bounds.h/2);
    SDL_RenderDrawRect(gRenderer, &sliderBounds);

    if (debug) drawBounds();
}

void GUISlider::tick()
{
    if (pressed)
    {
        if (mousePosX < bounds.x + sliderBounds.w/2) sliderBounds.x = bounds.x;
        else if (mousePosX > bounds.x + bounds.w - sliderBounds.w/2) sliderBounds.x = bounds.x + bounds.w - sliderBounds.w;
        else sliderBounds.x = mousePosX - sliderBounds.w/2;

        sliderVal = sliderBounds.x - bounds.x;
        float dinger = (float)sliderVal/bounds.w;
        func(dinger);
    }
}

void GUISlider::resetBounds()
{
    GUIObject::resetBounds();
    sliderBounds.x = bounds.x + sliderVal - sliderBounds.w/2;
    sliderBounds.y = bounds.y;
}
