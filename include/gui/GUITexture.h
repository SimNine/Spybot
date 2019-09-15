#ifndef GUITEXTURE_H
#define GUITEXTURE_H

#include <GUIObject.h>
#include <string>

class GUITexture : public GUIObject
{
    public:
        GUITexture(ANCHOR, int, int, SDL_Texture*, int, int, GUIContainer*);
        GUITexture(ANCHOR, int, int, SDL_Texture*, int, int, bool, GUIContainer*);
        GUITexture(ANCHOR, int, int, std::string, GUIContainer*);
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
