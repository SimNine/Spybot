#pragma once

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class GUIButton : public GUIObject {
public:
	// construct with strings
	GUIButton(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, void(*func) (void),
		std::string normal);
	GUIButton(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, void(*func) (void),
		std::string normal, std::string over);
	GUIButton(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, void(*func) (void),
		std::string normal, std::string over, std::string pressed);

	// construct with single string
	GUIButton(GUIContainer* parent, ANCHOR anchor, Coord disp, std::string text, int fontSize, void(*func) (void));

	// construct with no texture
	GUIButton(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, void(*func) (void));

	virtual ~GUIButton();

	void draw();
	bool mouseDown();
	bool mouseUp();
	void tick(int);
	void resetBounds();
	void setTransparency(int);
protected:
private:
	SDL_Texture* bkgNormal_;
	SDL_Texture* bkgOver_;
	SDL_Texture* bkgPressed_;
	void(*func) (void);
};