#ifndef GUITEXTURE_H
#define GUITEXTURE_H

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class GUITexture : public GUIObject
{
    public:
        GUITexture(ANCHOR a, Coord disp, SDL_Texture* tex, Coord, GUIContainer* parent);
        GUITexture(ANCHOR a, Coord disp, SDL_Texture* tex, Coord, bool, GUIContainer* parent);
        GUITexture(ANCHOR a, Coord disp, std::string addrOfTex, Coord dims, GUIContainer* parent);
        GUITexture(ANCHOR a, Coord disp, std::string strToTexturize, int fontSize, GUIContainer* parent);
        virtual ~GUITexture();

        SDL_Texture* swapTexture(SDL_Texture*);

        void draw();
        bool mouseDown();
        bool mouseUp();
        void setTransparency(int opacity);
        void tick(int ms);
        void resetBounds();
    protected:
    private:
        SDL_Texture* texture_;
        bool willDestroyTexture_;
};

#endif // GUITEXTURE_H
