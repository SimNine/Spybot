#include <SDL.h>
#include <string>

// loads a texture
SDL_Texture* loadTexture(std::string path);

// loads a music track
Mix_Music* loadMusic(std::string path);

// loads a sound effect
Mix_Chunk* loadSound(std::string path);
