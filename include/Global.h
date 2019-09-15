#ifndef GLOBAL_H
#define GLOBAL_H

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
extern Coord mousePos;
extern bool mousePressed;

// action flags
extern bool acceptingInput;

// quit flag
extern bool quit;

// debug level flag
extern DEBUG debug;

// programs owned in each campaign
extern int progListClassic[PROGRAM_NUM_PROGTYPES];
extern int progListNightfall[PROGRAM_NUM_PROGTYPES];
extern int progListCustom[PROGRAM_NUM_PROGTYPES];
extern int* progListCurrent;

// programs currently in play
extern int usedPrograms[PROGRAM_NUM_PROGTYPES];

// program ID currently selected (to be placed)
extern PROGRAM currProgram;

// GUIContainer currently held by the mouse
extern GUIContainer* heldContainer;

// credit counter
extern int numCredits;

// current profile
extern std::string profile;

#endif
