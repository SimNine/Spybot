#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>

#include "GUIContainer.h"
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

// closes SDL and SDL_image
void closeSDL()
{
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void resetBounds()
{
    titleScreen->resetBounds();
    mapScreen->resetBounds();
    gameScreen->resetBounds();
}

void initGUIs()
{
    SDL_Texture* titleBKG = loadTexture("resources/title_bkg.png");
    titleScreen = new GUIContainer(TOP_LEFT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, titleBKG);

    mapScreen = new MapScreen();
    gameScreen = new GameScreen();
}

// initializes SDL and SDL_image
bool initSDL()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    //Create window
    int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    gWindow = SDL_CreateWindow( "Spybot: The Nightfall Incident", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, flags );
    if( gWindow == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    SDL_SetWindowMinimumSize(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Create renderer for window
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    initData();
    initGUIs(); // initialize GUIContainers
    resetBounds();

    return true;
}

void draw()
{
    if (scrn == SCREEN_TITLE)
    {
        titleScreen->draw();
    }
    else if (scrn == SCREEN_MAP)
    {
        mapScreen->draw();
    }
    else if (scrn == SCREEN_GAME)
    {
        gameScreen->draw();
    }

    SDL_RenderPresent( gRenderer ); // update the screen
}

void handleEvents()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 )
    {
        // handle events not specific to a screen
        if( e.type == SDL_QUIT ) // window close
        {
            quit = true;
        }
        else if (e.type == SDL_WINDOWEVENT) // window resize
        {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                SCREEN_WIDTH = e.window.data1;
                SCREEN_HEIGHT = e.window.data2;
                resetBounds();
            }
        }
        else if (e.type == SDL_KEYDOWN) // keypress
        {
            if (e.key.keysym.sym == SDLK_F12) // toggle fullscreen on F12
            {
                if ((SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN) > 0)
                {
                    SDL_SetWindowFullscreen(gWindow, 0);
                }
                else
                {
                    SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                resetBounds();
            }
            else if (e.key.keysym.sym == SDLK_q)
            {
                scrn = SCREEN_TITLE;
            }
            else if (e.key.keysym.sym == SDLK_w)
            {
                scrn = SCREEN_MAP;
            }
            else if (e.key.keysym.sym == SDLK_e)
            {
                scrn = SCREEN_GAME;
            }
            else if (e.key.keysym.sym == SDLK_F2)
            {
                debug = !debug;
            }
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            mousePosX = e.motion.x;
            mousePosY = e.motion.y;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            mousePressed = true;
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            mousePressed = false;
        }

        // screen-specific input
        if (scrn == SCREEN_MAP)
        {
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                mapScreen->click();
            }
        }
        else if (scrn == SCREEN_TITLE)
        {
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                titleScreen->click();
            }
        }
        else if (scrn == SCREEN_GAME)
        {
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                gameScreen->click();
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s)
            {
                gameScreen->saveLevel();
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_l)
            {
                gameScreen->loadLevel("");
            }
        }

        // there was an event, so redraw the window
        nextDraw = true;
    }

    // handle in-progress events
    if (scrn == SCREEN_MAP)
    {
        // if the mapscreen is animating, don't take input
        if (mapScreen->isBusy())
        {
            return;
        }

        // scan for keys currently pressed
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if( currentKeyStates[ SDL_SCANCODE_UP ] )
        {
            mapScreen->shiftBkg(0, -2);
        }
        else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
        {
            mapScreen->shiftBkg(0, 2);
        }

        if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
        {
            mapScreen->shiftBkg(-2, 0);
        }
        else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
        {
            mapScreen->shiftBkg(2, 0);
        }

        // if the mouse is at an edge, try to shift the background
        if (mousePosX < 20)
        {
            mapScreen->shiftBkg(-1, 0);
        }
        else if (mousePosX > SCREEN_WIDTH - 20)
        {
            mapScreen->shiftBkg(1, 0);
        }

        if (mousePosY < 20)
        {
            mapScreen->shiftBkg(0, -1);
        }
        else if (mousePosY > SCREEN_HEIGHT - 20)
        {
            mapScreen->shiftBkg(0, 1);
        }
    }
    else if (scrn == SCREEN_GAME)
    {
        // scan for keys currently pressed
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if( currentKeyStates[ SDL_SCANCODE_UP ] )
        {
            gameScreen->shiftBkg(0, -2);
        }
        else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
        {
            gameScreen->shiftBkg(0, 2);
        }

        if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
        {
            gameScreen->shiftBkg(-2, 0);
        }
        else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
        {
            gameScreen->shiftBkg(2, 0);
        }

        // if the mouse is at an edge, try to shift the background
        if (mousePosX < 20)
        {
            gameScreen->shiftBkg(-1, 0);
        }
        else if (mousePosX > SCREEN_WIDTH - 20)
        {
            gameScreen->shiftBkg(1, 0);
        }

        if (mousePosY < 20)
        {
            gameScreen->shiftBkg(0, -1);
        }
        else if (mousePosY > SCREEN_HEIGHT - 20)
        {
            gameScreen->shiftBkg(0, 1);
        }
    }
}

void tick()
{

}

// main function
int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !initSDL() )
    {
        printf( "Failed to initialize SDL!\n" );
        return 1;
    }

    //Main loop
    while( !quit )
    {
        if (nextInput) handleEvents(); // handle input events
        if (nextTick) tick(); // tick whatever needs to be ticked
        if (nextDraw)
        {
            nextDraw = false; // set the window not to render again
            draw(); // render the window
        }
    }

    //Free resources, pointers, and close SDL
    closeSDL();

    return 0;
}
