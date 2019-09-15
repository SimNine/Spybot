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

#endif // RESOURCELOADER_H
