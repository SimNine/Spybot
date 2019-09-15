#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include <SDL.h>

#include "GUIObject.h"
#include "LinkedList.h"

class GUIContainer : public GUIObject
{
    public:
        GUIContainer(ANCHOR, Coord, Coord, GUIContainer*, SDL_Texture*);
        virtual ~GUIContainer();
        SDL_Texture* getBackground();
        void setBackground(SDL_Texture*);
        void addObject(GUIObject*);
        void addAllObjects(LinkedList<GUIObject*>*);
        LinkedList<GUIObject*>* getContents();
        void setPressed(bool);

        void resetBounds();
        void draw();
        bool mouseDown();
        bool mouseUp();
        void tick(int);
        void setTransparency(int);
    protected:
        SDL_Texture* bkgImg;
        void drawBkg();
        void drawContents();

        LinkedList<GUIObject*>* contents;
    private:
};

#endif // GUI_CONTAINER_H
