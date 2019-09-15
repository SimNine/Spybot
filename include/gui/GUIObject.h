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
        void resetBounds();
        int getXAnchor();
        int getYAnchor();
        bool isMouseOver();
        bool isVisible();
        void setVisible(bool);
        void setPressed(bool);

        // these to be overridden
        virtual void setTransparency(int) = 0;
        virtual bool mouseDown() = 0;
        virtual bool mouseUp() = 0;
        virtual void draw() = 0;
        virtual void tick() = 0;
    protected:
        int xDisplacement;
        int yDisplacement;
        ANCHOR anchor;
        SDL_Rect bounds;
        GUIContainer* parent;
        bool visible;
        bool pressed;
        void drawBounds();
    private:
};

#endif // GUIOBJECT_H
