#include "Global.h"
#include "MapScreen.h"

//Screen dimension constants
int SCREEN_WIDTH = 650;
int SCREEN_HEIGHT = 420;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

// designate the renderer
SDL_Renderer* gRenderer = NULL;

// designate the GUI_containers
screen scrn = SCREEN_MAP;
GUIContainer* titleScreen = NULL;
MapScreen* mapScreen = NULL;
GameScreen* gameScreen = NULL;

// designate texture container
DataContainer* dataContainer = NULL;

// mouse position
int mousePosX = 0;
int mousePosY = 0;
bool mousePressed = false;

// draw flag
bool nextDraw = false;
bool nextInput = true;
bool nextTick = true;

// quit flag
bool quit = false;

// debug flag
bool debug = false;
