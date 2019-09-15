#include <SDL.h>

#include "GUIContainer.h"
#include "DataContainer.h"
#include "MapScreen.h"
#include "GameScreen.h"
#include "TitleScreen.h"
#include "MainScreen.h"

// screen dimensions
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

// window to render in
extern SDL_Window* gWindow;

// designate the renderer
extern SDL_Renderer* gRenderer;

// designate the GUI_containers
extern GUIContainer* currScreen;
extern TitleScreen* titleScreen;
extern MainScreen* mainScreen;
extern MapScreen* mapScreen;
extern GameScreen* gameScreen;

// container to hold pointers to node textures
extern DataContainer* dataContainer;

// mouse data
extern int mousePosX;
extern int mousePosY;
extern bool mousePressed;

// action flags
extern bool acceptingInput;

// quit flag
extern bool quit;

// debug flag
extern bool debug;
