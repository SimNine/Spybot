#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include "Standard.h"

class GUIContainer;

class GUIObject
{
    public:
        GUIObject(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent);
        virtual ~GUIObject();
        SDL_Rect* getBounds();
        void setBounds(Coord disp, Coord dims);
        int getXAnchor();
        int getYAnchor();
        bool isMouseOver();
        bool isVisible();
        void setPressed(bool pressed);
        void fade(int endAlpha, int fadeDuration);
        void fadeStep(int);
        int getTransparency();

        void recomputePosition();

        // these to be overridden
        virtual void setTransparency(int) = 0;
        virtual bool mouseDown() = 0;
        virtual bool mouseUp() = 0;
        virtual void draw() = 0;
        virtual void tick(int millisec) = 0;
        virtual void resetBounds() = 0;
    protected:
        Coord displacement;
        ANCHOR anchor;
        SDL_Rect bounds;
        GUIContainer* parent;
        bool pressed;
        void drawBounds();

        int currAlpha;
        int endAlpha;
        int startAlpha;
        int fadeInitDuration;
        int fadeCurrDuration;
    private:
};

#endif // GUIOBJECT_H
