#pragma once

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class GUITexture : public GUIObject {
public:
	// construct from preallocated SDL_Texture
	GUITexture(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims, SDL_Texture* tex);
	GUITexture(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims, SDL_Texture* tex, bool willDestroy);

	// construct by loading own SDL_Texture
	GUITexture(GUIContainer* parent, ANCHOR a, Coord disp, Coord dims, std::string addrOfTex);
	GUITexture(GUIContainer* parent, ANCHOR a, Coord disp, std::string addrOfTex);

	// construct from string
	GUITexture(GUIContainer* parent, ANCHOR a, Coord disp, std::string strToTexturize, int fontSize);

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