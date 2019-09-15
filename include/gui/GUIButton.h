#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <GUIObject.h>
#include <string>

class GUIButton : public GUIObject
{
    public:
        GUIButton(ANCHOR, int, int, std::string, GUIContainer*,
                  void (*func) (void));
        GUIButton(ANCHOR, int, int, int, int, GUIContainer*,
                  void (*func) (void), SDL_Texture*, SDL_Texture*, SDL_Texture*);
        GUIButton(ANCHOR, int, int, int, int, GUIContainer*,
                  void (*func) (void), SDL_Texture*, SDL_Texture*);
        GUIButton(ANCHOR, int, int, int, int, GUIContainer*,
                  void (*func) (void), SDL_Texture*);
        GUIButton(ANCHOR, int, int, int, int, GUIContainer*,
                  void (*func) (void));
        virtual ~GUIButton();

        void draw();
        bool mouseDown();
        bool mouseUp();
        void tick(int);
        void resetBounds();
        void setTransparency(int);
    protected:
    private:
        SDL_Texture* bkgNormal;
        SDL_Texture* bkgOver;
        SDL_Texture* bkgPressed;
        void (*func) (void);
};

#endif // GUIBUTTON_H
