#ifndef GUISLIDER_H
#define GUISLIDER_H

#include "GUIObject.h"

class GUISlider : public GUIObject
{
    public:
        GUISlider(ANCHOR, Coord, Coord, GUIContainer*, void (*func) (float));
        virtual ~GUISlider();

        void setTransparency(int);
        bool mouseDown();
        bool mouseUp();
        void draw();
        void tick(int);
        void resetBounds();
    protected:
    private:
        SDL_Rect sliderBounds;
        int sliderVal;
        void (*func) (float);
};

#endif // GUISLIDER_H
