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
DEBUG debug = DEBUG_MINIMAL;

// owned programs per campaign
int classicPrograms[PROGRAM_NUM_PROGTYPES];
int nightfallPrograms[PROGRAM_NUM_PROGTYPES];
int customPrograms[PROGRAM_NUM_PROGTYPES];

// programs currently in play
int usedPrograms[PROGRAM_NUM_PROGTYPES];

// program ID currently selected (to be placed)
PROGRAM currProgram = PROGRAM_NONE;
