#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "Standard.h"

// loads a texture
SDL_Texture* loadTexture(std::string path);

// loads a music track
Mix_Music* loadMusic(std::string path);

// loads a sound effect
Mix_Chunk* loadSound(std::string path);

// loads a texture of a specific font and size
SDL_Texture* loadString(std::string str, FONT, int, SDL_Color);

// renders a string, immediately deletes the texture
void drawString(std::string str, FONT ft, int fsize, SDL_Color col, SDL_Rect bounds);

#endif // RESOURCELOADER_H
