#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class GUIButton : public GUIObject
{
    public:
		// construct with textures
		GUIButton(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent,
				  void(*func) (void), SDL_Texture* normal);
		GUIButton(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent,
				  void(*func) (void), SDL_Texture* normal, SDL_Texture* over);
		GUIButton(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent,
				  void(*func) (void), SDL_Texture* normal, SDL_Texture* over, SDL_Texture* pressed);

		// construct with strings
		GUIButton(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent,
			void(*func) (void), std::string normal);
		GUIButton(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent,
			void(*func) (void), std::string normal, std::string over);
		GUIButton(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent,
			void(*func) (void), std::string normal, std::string over, std::string pressed);

		// construct with single string
        GUIButton(ANCHOR anchor, Coord disp, std::string text, GUIContainer* parent,
                  void (*func) (void));

		// construct with no texture
        GUIButton(ANCHOR anchor, Coord disp, Coord dims, GUIContainer* parent,
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
