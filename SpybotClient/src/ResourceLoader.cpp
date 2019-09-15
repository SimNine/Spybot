#include "Standard.h"
#include "ResourceLoader.h"

#include "Global.h"

// returns a pointer to a new SDL_Texture
SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

Mix_Music* loadMusic(std::string path)
{
    Mix_Music* gMusic = Mix_LoadMUS(path.c_str());
    if( gMusic == NULL )
    {
        printf( "Failed to load music at %s! SDL_mixer Error: %s\n", path.c_str(), Mix_GetError() );
    }
    return gMusic;
}

Mix_Chunk* loadSound(std::string path)
{
    Mix_Chunk* gChunk = Mix_LoadWAV(path.c_str());
    if( gChunk == NULL )
    {
        printf( "Failed to load sound at %s! SDL_mixer Error: %s\n", path.c_str(), Mix_GetError() );
    }
    return gChunk;
}

SDL_Texture* loadString(std::string str, FONT ft, int fsize, SDL_Color col)
{
    TTF_Font* f;
    if (ft == FONT_NORMAL) f = TTF_OpenFont("resources/AGENCYR.ttf", fsize);
    else if (ft == FONT_BOLD) f = TTF_OpenFont("resources/AGENCYB.ttf", fsize);
	else f = TTF_OpenFont("resources/AGENCYR.ttf", fsize);
    if (!f) printf("TTF_OpenFont: %s\n", TTF_GetError());

    SDL_Surface* s = TTF_RenderText_Blended(f, str.c_str(), col);
    SDL_Texture* t = SDL_CreateTextureFromSurface(gRenderer, s);

    TTF_CloseFont(f);
    SDL_FreeSurface(s);
    return t;
}