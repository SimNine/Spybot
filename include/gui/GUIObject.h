#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include <SDL.h>

#include "Enums.h"

class GUIContainer;

class GUIObject
{
    public:
        GUIObject(ANCHOR, int, int, int, int, GUIContainer*);
        virtual ~GUIObject();
        SDL_Rect* getBounds();
        void setBounds(int, int, int, int);
        int getXAnchor();
        int getYAnchor();
        bool isMouseOver();
        bool isVisible();
        void setPressed(bool);
        void fade(int, int);
        void fadeStep(int);
        int getTransparency();

        void recomputePosition();

        // these to be overridden
        virtual void setTransparency(int) = 0;
        virtual bool mouseDown() = 0;
        virtual bool mouseUp() = 0;
        virtual void draw() = 0;
        virtual void tick(int) = 0;
        virtual void resetBounds() = 0;
    protected:
        int xDisplacement;
        int yDisplacement;
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
