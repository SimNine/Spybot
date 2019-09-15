#ifndef GUITEXTURE_H
#define GUITEXTURE_H

#include <GUIObject.h>


class GUITexture : public GUIObject
{
    public:
        GUITexture(ANCHOR, int, int, SDL_Texture*, int, int, GUIContainer*);
        virtual ~GUITexture();
        void draw();
        bool mouseDown();
        bool mouseUp();
        void setTransparency(int);
        void tick();
    protected:
    private:
        SDL_Texture* texture;
};

#endif // GUITEXTURE_H
