#include "Global.h"
#include "MapScreen.h"

// initial screen dimension constants
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// window to render to
SDL_Window* gWindow = NULL;

// designate the renderer
SDL_Renderer* gRenderer = NULL;

// designate the GUI_containers
GUIContainer* currScreen = NULL;
TitleScreen* titleScreen = NULL;
MainScreen* mainScreen = NULL;
MapScreen* mapScreen = NULL;
GameScreen* gameScreen = NULL;

// designate texture container
DataContainer* dataContainer = NULL;

// mouse position
int mousePosX = 0;
int mousePosY = 0;
bool mousePressed = false;

// update flags
bool acceptingInput = true;

// quit flag
bool quit = false;

// debug flag
bool debug = false;
