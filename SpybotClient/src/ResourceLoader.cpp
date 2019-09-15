#include "Standard.h"
#include "ResourceLoader.h"

#include "Global.h"

// returns a pointer to a new SDL_Texture
SDL_Texture* loadTexture(std::string path) {
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		log("Unable to load image \"" + path + "\"! SDL_image Error: " + std::string(IMG_GetError()) + "\n");
	} else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
		if (newTexture == NULL) {
			log("Unable to create texture from \"" + path + "\"! SDL Error: " + std::string(SDL_GetError()) + "\n");
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

Mix_Music* loadMusic(std::string path) {
	Mix_Music* gMusic = Mix_LoadMUS(path.c_str());
	if (gMusic == NULL) {
		log("Failed to load music at " + path + "! SDL_mixer Error: " + std::string(Mix_GetError()) + "\n");
	}
	return gMusic;
}

Mix_Chunk* loadSound(std::string path) {
	Mix_Chunk* gChunk = Mix_LoadWAV(path.c_str());
	if (gChunk == NULL) {
		log("Failed to load sound at " + path + "! SDL_mixer Error: " + std::string(Mix_GetError()) + "\n");
	}
	return gChunk;
}

SDL_Texture* loadString(std::string str, FONT ft, int fsize, SDL_Color col) {
	// load the font in the correct size
	TTF_Font* f;
	if (ft == FONT_NORMAL) 
		f = TTF_OpenFont("resources/AGENCYR.ttf", fsize);
	else if (ft == FONT_BOLD) 
		f = TTF_OpenFont("resources/AGENCYB.ttf", fsize);
	else 
		f = TTF_OpenFont("resources/AGENCYR.ttf", fsize);
	if (!f) 
		log("TTF_OpenFont: " + std::string(TTF_GetError()) + "\n");

	// load the surface
	SDL_Surface* s = TTF_RenderText_Blended(f, str.c_str(), col);
	if (s == NULL) {
		TTF_CloseFont(f);
		return NULL;
	}

	// load the texture
	SDL_Texture* t = SDL_CreateTextureFromSurface(_renderer, s);
	SDL_SetTextureAlphaMod(t, col.a);

	// free and return
	TTF_CloseFont(f);
	SDL_FreeSurface(s);
	return t;
}

void drawString(std::string str, FONT ft, int fsize, SDL_Color col, SDL_Rect bounds) {
	SDL_Texture* tex = loadString(str, ft, fsize, col);
	SDL_QueryTexture(tex, NULL, NULL, &bounds.w, &bounds.h);
	SDL_RenderCopy(_renderer, tex, NULL, &bounds);
	SDL_DestroyTexture(tex);
}