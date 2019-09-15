#include "Standard.h"
#include "Main.h"

#include "Global.h"
#include "TitleScreen.h"
#include "MainScreen.h"
#include "MapScreen.h"
#include "GameScreen.h"
#include "LobbyScreen.h"
#include "NotifyScreen.h"
#include "DataContainer.h"
#include "Client.h"
#include "Timer.h"
#include "ChatDisplay.h"

void savePrefs();
void loadPrefs();

void saveProgs();
void loadProgs();

// converts an SDL_Keycode into a char
char convertSDLKeycodeToChar(SDL_Keycode keycode, bool shift)
{
	const char* k = SDL_GetKeyName(keycode);
	if (strlen(k) == 1)
	{
		// get the key
		char key = k[0];

		// if alphabetical
		if (key >= 60 && key <= 90)
			return shift ? key : key + 32;

		// other regular chars
		switch (key) {
		case '1':
			return shift ? '!' : '1';
		case '2':
			return shift ? '@' : '2';
		case '3':
			return shift ? '#' : '3';
		case '4':
			return shift ? '$' : '4';
		case '5':
			return shift ? '%' : '5';
		case '6':
			return shift ? '^' : '6';
		case '7':
			return shift ? '&' : '7';
		case '8':
			return shift ? '*' : '8';
		case '9':
			return shift ? '(' : '9';
		case '0':
			return shift ? ')' : '0';
		case '/':
			return shift ? '?' : '/';
		case '.':
			return shift ? '>' : '.';
		case ',':
			return shift ? '<' : ',';
		case '-':
			return shift ? '_' : '-';
		case '=':
			return shift ? '+' : '=';
		case '\'':
			return shift ? '\"' : '\'';
		case ';':
			return shift ? ':' : ';';
		default:
			return 0;
		}
	}
	else if (keycode == SDLK_SPACE)
		return ' ';
	else if (keycode == SDLK_BACKSPACE)
		return 127;
	else if (keycode == SDLK_RETURN)
		return 13;
	else if (keycode == SDLK_ESCAPE)
		return 27;
	else 
		return 0;
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
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// resets all GUI bounds
void resetBounds()
{
    titleScreen->resetBounds();
    mainScreen->resetBounds();
    mapScreen->resetBounds();
    gameScreen->resetBounds();
	lobbyScreen->resetBounds();
	notifyScreen->resetBounds();
}

// initializes the GUIs and switches the screen to titleScreen
void initGUIs()
{
    titleScreen = new TitleScreen();
    mainScreen = new MainScreen();
    mapScreen = new MapScreen();
	mapScreen->switchMap(MAPPRESET_CLASSIC);
    gameScreen = new GameScreen();
	lobbyScreen = new LobbyScreen();
	notifyScreen = new NotifyScreen();

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

// initialize winsock
bool initWinsock() {

	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	// initialize the client entity
	client = new Client();

	return true;
}

// toggle fullscreen on or off
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

// save the user's preferences
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

// load the user's preferences
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

// save the user's programs
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
            progs.write((char*) &progListClassic[i], sizeOfInt);
            progs.write((char*) &progListNightfall[i], sizeOfInt);
            progs.write((char*) &progListCustom[i], sizeOfInt);
        }

        // flush and close
        if (debug >= DEBUG_MINIMAL) printf("flushing and closing programs file... ");
        progs.flush();
        progs.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

// load the user's programs
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
            progs.read((char*) &progListClassic[i], sizeOfInt);
            progs.read((char*) &progListNightfall[i], sizeOfInt);
            progs.read((char*) &progListCustom[i], sizeOfInt);
            usedPrograms[i] = 0;
        }

        // close the file
        progs.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

// handle all events in SDL event queue
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
            else if (e.key.keysym.sym == SDLK_F2)
            {
                if (debug == DEBUG_EXTRA) debug = DEBUG_NONE;
                else debug = (DEBUG)(debug + 1);
            }
            else if (e.key.keysym.sym == SDLK_p)
            {
                progListCurrent[rand()%PROGRAM_NUM_PROGTYPES]++;
                mapScreen->resetProgramInvDisplay();
            }
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            if (heldContainer != NULL)
            {
                Coord delta = {e.motion.x - mousePos.x, e.motion.y - mousePos.y};
                if (debug >= DEBUG_EXTRA) printf("%i,%i\n", delta.x, delta.y);
                heldContainer->incDisplacement(delta);
            }

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
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_s)
                {
                    gameScreen->saveGame();
                }
                else if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    gameScreen->togglePauseMenu();
                }
                else if (e.key.keysym.sym == SDLK_F3)
                {
                    gameScreen->toggleEditorMode();
                }

				// handle chat display
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					gameScreen->getChatDisplay()->addInputChar(out);
            }
        }
        else if (currScreen == mapScreen)
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_l)
                {
                    mapScreen->unlockAllLevels();
                }
                else if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    mapScreen->togglePauseMenu();
                }
            }
        }
		else if (currScreen == mainScreen)
		{
			if (e.type == SDL_KEYDOWN)
			{
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					mainScreen->keyPress(out);
			}
		}
		else if (currScreen == lobbyScreen)
		{
			if (e.type == SDL_KEYDOWN)
			{
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					lobbyScreen->getChatDisplay()->addInputChar(out);
			}
		}
	}
}

// draw the current screen
void draw()
{
	currScreen->draw();
	notifyScreen->draw();
	SDL_RenderPresent(gRenderer); // update the screen
}

// tick the current screen
void tick(int ms)
{
    currScreen->tick(ms);
	notifyScreen->tick(ms);
}

// main function
int main( int argc, char* args[] )
{
	// start up winsock and create the client
	if (!initWinsock())
	{
		printf("Failed to initialize Winsock\n");
	}

    // start up SDL and create window
    if( !initSDL() )
    {
        printf("Failed to initialize SDL\n");
        return 1;
    }

    // setup timers
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
		// process messages
		client->processAllMessages();

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
                float fps = (float)(frameCount / (numTicks/1000.0));
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
