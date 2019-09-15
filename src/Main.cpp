#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_TTF.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <fstream>
#include <stdio.h>

#include "GUIContainer.h"
#include "Global.h"
#include "TitleScreen.h"
#include "Timer.h"
#include "MiscUtil.h"

void savePrefs();
void loadPrefs();

void saveProgs();
void loadProgs();

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

    loadPrefs();
    loadProgs();
    initData();
    initGUIs(); // initialize GUIContainers
    resetBounds();

    return true;
}

void toggleFullscreen()
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

void draw()
{
    currScreen->draw();
    SDL_RenderPresent( gRenderer ); // update the screen
}

void savePrefs()
{
    std::ofstream prefs;
    prefs.open("default.prefs", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!prefs.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err saving prefs\n");
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("saving prefs...\n");

        // begin by writing the sizes of various data types
        int8_t sizeOfInt = sizeof(int);
        int8_t sizeOfChar = sizeof(char);
        int8_t sizeOfDouble = sizeof(double);
        int8_t sizeOfBool = sizeof(bool);
        if (debug >= DEBUG_NORMAL) printf("saving constants... int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);
        prefs.write((char*) &sizeOfInt, 1);
        prefs.write((char*) &sizeOfChar, 1);
        prefs.write((char*) &sizeOfDouble, 1);
        prefs.write((char*) &sizeOfBool, 1);

        // save preferences
        int soundVol = Mix_Volume(-1, 0);
        int musicVol = Mix_VolumeMusic(0);
        Uint32 windowFlags = SDL_GetWindowFlags(gWindow);
        if (debug >= DEBUG_NORMAL) printf("saving preferences... soundVol:%i, musicVol:%i, SCRWIDTH:%i, SCRHEIGHT:%i\n",
                          soundVol, musicVol, SCREEN_WIDTH, SCREEN_HEIGHT);
        prefs.write((char*) &soundVol, sizeOfInt);
        prefs.write((char*) &musicVol, sizeOfInt);
        prefs.write((char*) &windowFlags, sizeof(Uint32));
        prefs.write((char*) &SCREEN_WIDTH, sizeOfInt);
        prefs.write((char*) &SCREEN_HEIGHT, sizeOfInt);

        // flush and close
        if (debug >= DEBUG_MINIMAL) printf("flushing and closing prefs file... ");
        prefs.flush();
        prefs.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void loadPrefs()
{
    std::ifstream prefs;
    prefs.open("default.prefs", std::ios::in | std::ios::binary);

    if (!prefs.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err reading prefs\n");
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("reading prefs...\n");

        // read the sizes of various data types
        if (debug >= DEBUG_NORMAL) printf("loading constants...\n");
        int8_t sizeOfInt;
        prefs.read((char*) &sizeOfInt, 1);
        int8_t sizeOfChar;
        prefs.read((char*) &sizeOfChar, 1);
        int8_t sizeOfDouble;
        prefs.read((char*) &sizeOfDouble, 1);
        int8_t sizeOfBool;
        prefs.read((char*) &sizeOfBool, 1);
        if (debug >= DEBUG_NORMAL) printf("loaded constants: int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);

        // read prefs
        if (debug >= DEBUG_NORMAL) printf("loading prefs...\n");
        int soundVol;
        prefs.read((char*) &soundVol, sizeOfInt);
        Mix_Volume(-1, soundVol);
        int musicVol;
        prefs.read((char*) &musicVol, sizeOfInt);
        Mix_VolumeMusic(musicVol);
        int windowFlags;
        prefs.read((char*) &windowFlags, sizeof(Uint32));
        if (windowFlags & SDL_WINDOW_FULLSCREEN) SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        prefs.read((char*) &SCREEN_WIDTH, sizeOfInt);
        prefs.read((char*) &SCREEN_HEIGHT, sizeOfInt);
        SDL_SetWindowSize(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
        if (debug >= DEBUG_NORMAL) printf("loaded prefs: soundVol:%i, musicVol:%i, fullscreen:%i, SCRWIDTH:%i, SCRHEIGHT:%i\n",
                          soundVol, musicVol, windowFlags, SCREEN_WIDTH, SCREEN_HEIGHT);

        // close the file
        prefs.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void saveProgs()
{
    std::ofstream progs;
    progs.open("default.progs", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!progs.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err saving programs\n");
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("saving programs...\n");

        // save constants
        int8_t sizeOfInt = sizeof(int);
        progs.write((char*) &sizeOfInt, 1);

        // save programs
        for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
        {
            progs.write((char*) &classicPrograms[i], sizeOfInt);
            progs.write((char*) &nightfallPrograms[i], sizeOfInt);
            progs.write((char*) &customPrograms[i], sizeOfInt);
        }

        // flush and close
        if (debug >= DEBUG_MINIMAL) printf("flushing and closing programs file... ");
        progs.flush();
        progs.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void loadProgs()
{
    std::ifstream progs;
    progs.open("default.progs", std::ios::in | std::ios::binary);

    if (!progs.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err reading programs\n");
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("reading programs...\n");

        // load constants
        int8_t sizeOfInt;
        progs.read((char*) &sizeOfInt, 1);

        // load programs
        for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
        {
            progs.read((char*) &classicPrograms[i], sizeOfInt);
            progs.read((char*) &nightfallPrograms[i], sizeOfInt);
            progs.read((char*) &customPrograms[i], sizeOfInt);
            usedPrograms[i] = 0;
        }

        // close the file
        progs.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
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
                toggleFullscreen();
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
                if (debug == DEBUG_EXTRA) debug = DEBUG_NONE;
                else debug = (DEBUG)(debug + 1);
            }
            else if (e.key.keysym.sym == SDLK_F3 && currScreen == gameScreen)
            {
                gameScreen->toggleEditorMode();
            }
            else if (e.key.keysym.sym == SDLK_p)
            {
                classicPrograms[rand()%PROGRAM_NUM_PROGTYPES]++;
                mapScreen->resetProgramInvDisplay();
            }
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            mousePos.x = e.motion.x;
            mousePos.y = e.motion.y;
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
                gameScreen->saveGame();
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_l)
            {
                gameScreen->loadGame("");
            }
        }
        else if (currScreen == mapScreen)
        {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_l)
            {
                mapScreen->unlockAllLevels();
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

        if (debug >= DEBUG_EXTRA)
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
    savePrefs();
    saveProgs();
    closeSDL();

    return 0;
}
