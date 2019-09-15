#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <GUIObject.h>


class GUIButton : public GUIObject
{
    public:
        GUIButton(Anch, int, int, int, int, GUIContainer*,
                  void (*func) (void), SDL_Texture*, SDL_Texture*, SDL_Texture*);
        GUIButton(Anch, int, int, int, int, GUIContainer*,
                  void (*func) (void), SDL_Texture*, SDL_Texture*);
        GUIButton(Anch, int, int, int, int, GUIContainer*,
                  void (*func) (void), SDL_Texture*);
        GUIButton(Anch, int, int, int, int, GUIContainer*,
                  void (*func) (void));
        virtual ~GUIButton();
        void draw();
        bool mouseDown();
        bool mouseUp();
        void tick();
        void setTransparency(int);
    protected:
    private:
        SDL_Texture* bkgNormal;
        SDL_Texture* bkgOver;
        SDL_Texture* bkgPressed;
        void (*func) (void);
};

#endif // GUIBUTTON_H