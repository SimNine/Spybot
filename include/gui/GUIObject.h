#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include <SDL.h>

enum Anch { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, CENTER };

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
        bool getVisible();
        void setVisible(bool);

        // these to be overridden
        virtual bool click() = 0;
        virtual void draw() = 0;
    protected:
        int xDisplacement;
        int yDisplacement;
        Anch anchor;
        SDL_Rect* bounds;
        GUIContainer* parent;
        bool visible;
    private:
};

#endif // GUIOBJECT_H
