#ifndef GUISLIDER_H
#define GUISLIDER_H

#include "GUIObject.h"

class GUISlider : public GUIObject
{
    public:
        GUISlider(Anch, int, int, int, int, GUIContainer*, void (*func) (float));
        virtual ~GUISlider();
        void setTransparency(int);
        bool mouseDown();
        bool mouseUp();
        void draw();
        void tick();
        void resetBounds();
    protected:
    private:
        int transparency;
        SDL_Rect sliderBounds;
        int sliderVal;
        void (*func) (float);
};

#endif // GUISLIDER_H
