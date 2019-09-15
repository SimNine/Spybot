#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <GUIObject.h>
#include <string>

class GUIButton : public GUIObject
{
    public:
        GUIButton(ANCHOR, Coord, std::string, GUIContainer*,
                  void (*func) (void));
        GUIButton(ANCHOR, Coord, Coord, GUIContainer*,
                  void (*func) (void), std::string, std::string, std::string);
        GUIButton(ANCHOR, Coord, Coord, GUIContainer*,
                  void (*func) (void), std::string, std::string);
        GUIButton(ANCHOR, Coord, Coord, GUIContainer*,
                  void (*func) (void), std::string);
        GUIButton(ANCHOR, Coord, Coord, GUIContainer*,
                  void (*func) (void), SDL_Texture*);
        GUIButton(ANCHOR, Coord, Coord, GUIContainer*,
                  void (*func) (void), SDL_Texture*, SDL_Texture*);
        GUIButton(ANCHOR, Coord, Coord, GUIContainer*,
                  void (*func) (void), SDL_Texture*, SDL_Texture*, SDL_Texture*);
        GUIButton(ANCHOR, Coord, Coord, GUIContainer*,
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
        bool willDestroyTextures;
};

#endif // GUIBUTTON_H
