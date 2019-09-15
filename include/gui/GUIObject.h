#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include <SDL.h>

enum Anch { ANCHOR_TOP_LEFT, ANCHOR_TOP_RIGHT, ANCHOR_BOTTOM_LEFT, ANCHOR_BOTTOM_RIGHT, ANCHOR_CENTER };

class GUIContainer;

class GUIObject
{
    public:
        GUIObject(Anch, int, int, int, int, GUIContainer*);
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
        Anch anchor;
        SDL_Rect bounds;
        GUIContainer* parent;
        bool visible;
        bool pressed;
        void drawBounds();
    private:
};

#endif // GUIOBJECT_H
