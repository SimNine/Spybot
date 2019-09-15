#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include "Standard.h"
#include "GUIObject.h"
#include "LinkedList.h"

class GUIContainer : public GUIObject
{
    public:
        GUIContainer(GUIContainer* parent, ANCHOR anch, Coord disp, Coord dims, SDL_Texture* bkg);
		GUIContainer(GUIContainer* parent, ANCHOR anch, Coord disp, Coord dims, SDL_Color col);
        GUIContainer(GUIContainer* parent, ANCHOR anch, Coord disp, SDL_Texture* bkg);

        virtual ~GUIContainer();
        SDL_Texture* getBackgroundImg();
        SDL_Color getBackgroundCol();
        void setBackgroundImg(SDL_Texture*);
        void setBackgroundCol(SDL_Color);
        void addObject(GUIObject*);
        void removeObject(GUIObject*);
        void addAllObjects(LinkedList<GUIObject*>*);
        LinkedList<GUIObject*>* getContents();
        void setPressed(bool);

        void resetBounds();
        void draw();
        bool mouseDown();
        bool mouseUp();
        void tick(int);
        void setTransparency(int);

        void incDisplacement(Coord);

        bool isMovable();
        void setMovable(bool);

    protected:
        SDL_Color* bkgCol;
        SDL_Texture* bkgImg;
        void drawBkg();
        void drawContents();

        bool movable;

        LinkedList<GUIObject*>* contents;

    private:
};

#endif // GUI_CONTAINER_H
