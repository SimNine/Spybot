#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include <SDL.h>

#include "GUIObject.h"
#include "LinkedList.h"

class GUIContainer : public GUIObject
{
    public:
        GUIContainer(ANCHOR, int, int, int, int, GUIContainer*, SDL_Texture*);
        virtual ~GUIContainer();
        SDL_Texture* getBackground();
        void setBackground(SDL_Texture*);
        LinkedList<GUIObject*>* getContents();
        void resetBounds();
        void addObject(GUIObject*);
        void addAllObjects(LinkedList<GUIObject*>*);
        void draw();
        bool mouseDown();
        bool mouseUp();
        void tick();
        void setTransparency(int);
    protected:
        SDL_Texture* bkgImg;
        LinkedList<GUIObject*>* contents;
        void drawBkg();
        void drawContents();
    private:
};

#endif // GUI_CONTAINER_H
