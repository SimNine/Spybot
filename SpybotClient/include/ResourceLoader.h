#pragma once

#include "Standard.h"

// loads a texture
extern SDL_Texture* loadTexture(std::string path);

// loads a music track
extern Mix_Music* loadMusic(std::string path);

// loads a sound effect
extern Mix_Chunk* loadSound(std::string path);

// loads a texture of a specific font and size
extern SDL_Texture* loadString(std::string str, FONT, int, SDL_Color);

// renders a string, immediately deletes the texture
extern void drawString(std::string str, FONT ft, int fsize, SDL_Color col, SDL_Rect bounds);