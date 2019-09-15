#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_TTF.h>
#include <stdio.h>
#include <string>
#include <cmath>

#include "GUIContainer.h"
#include "Global.h"
#include "TitleScreen.h"
#include "Timer.h"

// closes SDL and SDL_image
void closeSDL()
{
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void resetBounds()
{
    titleScreen->resetBounds();
    mainScreen->resetBounds();
    mapScreen->resetBounds();
    gameScreen->resetBounds();
}

void initGUIs()
{
    titleScreen = new TitleScreen();
    mainScreen = new MainScreen();
    mapScreen = new MapScreen();
    gameScreen = new GameScreen();

    currScreen = titleScreen;
}

// initializes SDL and SDL_image
bool initSDL()
{
    // set non-thread-naming hint
    if( !SDL_SetHint( SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1" ) )
    {
        printf( "Warning: thread non-naming failed to enable!" );
    }

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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
    int flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    gWindow = SDL_CreateWindow( "Spybot: The Nightfall Incident", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, flags );
    if( gWindow == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    SDL_SetWindowMinimumSize(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_ShowCursor(SDL_DISABLE);

    //Create renderer for window
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    // initialize TTF
    if ( TTF_Init() != 0 )
    {
        printf("SDL_TTF could not initialize! SDL_TTF error: %s\n", TTF_GetError());
        return false;
    }

    initData();
    initGUIs(); // initialize GUIContainers
    resetBounds();

    return true;
}

void draw()
{
    currScreen->draw();
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
                currScreen = mainScreen;
            }
            else if (e.key.keysym.sym == SDLK_w)
            {
                currScreen = mapScreen;
            }
            else if (e.key.keysym.sym == SDLK_e)
            {
                currScreen = gameScreen;
            }
            else if (e.key.keysym.sym == SDLK_r)
            {
                currScreen = titleScreen;
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
            currScreen->mouseDown();
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            mousePressed = false;
            currScreen->mouseUp();
        }

        // screen-specific input
        if (currScreen == gameScreen)
        {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s)
            {
                gameScreen->saveLevel();
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_l)
            {
                gameScreen->loadLevel("");
            }
        }
    }
}

void tick(int ms)
{
    currScreen->tick(ms);
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

    // timers
    Timer tickTimer;
    tickTimer.start();

    Timer renderTimer;
    renderTimer.start();

    unsigned int fpsCap = 60;
    unsigned int tickCap = 60;
    unsigned int frameCount = 0;
    unsigned int tickCount = 0;
    unsigned int msPerFrame;
    unsigned int msPerTick;
    Timer renderTimerB;
    Timer tickTimerB;
    if (fpsCap != 0)
    {
        renderTimerB.start();
        msPerFrame = 1000/fpsCap;
    }
    if (tickCap != 0)
    {
        tickTimerB.start();
        msPerTick = 1000/tickCap;
    }

    // main loop
    SDL_ShowWindow(gWindow);
    while( !quit )
    {
        // handle ticks
        unsigned int ticksPassed = tickTimer.getTicks();
        if (tickCap == 0)
        {
            tickTimer.stop();
            tickTimer.start();
            tick(ticksPassed); // tick whatever needs to be ticked
            if (acceptingInput) handleEvents();
            tickCount++;
        }
        if (ticksPassed >= msPerTick) // tick whenever 20+ ms have gone by
        {
            tickTimer.stop();
            tickTimer.start();
            tick(ticksPassed); // tick whatever needs to be ticked
            if (acceptingInput) handleEvents();
            tickCount++;
        }

        // handle drawing
        if (fpsCap == 0) // if there is no fps cap
        {
            draw(); // render the window
            frameCount++;
        }
        else if (renderTimerB.getTicks() >= msPerFrame) // if there is an fps cap and enough ms have gone
        {
            draw();
            frameCount++;
            renderTimerB.stop();
            renderTimerB.start();
        }

        if (debug)
        {
            unsigned int numTicks = renderTimer.getTicks();
            if (numTicks >= 1000)
            {
                float fps = frameCount / (numTicks/1000.0);
                renderTimer.stop();
                printf("avg fps: %f\n", fps);
                printf("num frames: %i\n\n", frameCount);
                frameCount = 0;
                renderTimer.start();
            }
        }
    }

    //Free resources, pointers, and close SDL
    closeSDL();

    return 0;
}
