#ifndef GUITEXTURE_H
#define GUITEXTURE_H

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class GUITexture : public GUIObject
{
    public:
        GUITexture(ANCHOR, Coord, SDL_Texture*, Coord, GUIContainer*);
        GUITexture(ANCHOR, Coord, SDL_Texture*, Coord, bool, GUIContainer*);
        GUITexture(ANCHOR, Coord, std::string, Coord, GUIContainer*);
        GUITexture(ANCHOR, Coord, std::string, GUIContainer*);
        virtual ~GUITexture();

        SDL_Texture* swapTexture(SDL_Texture*);

        void draw();
        bool mouseDown();
        bool mouseUp();
        void setTransparency(int);
        void tick(int);
        void resetBounds();
    protected:
    private:
        SDL_Texture* texture;
        bool willDestroyTexture;
};

#endif // GUITEXTURE_H
