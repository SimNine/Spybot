#ifndef GLOBAL_H
#define GLOBAL_H

#include "Standard.h"

class GUIContainer;
class TitleScreen;
class MainScreen;
class MapScreen;
class GameScreen;
class LobbyScreen;
class NotifyScreen;
class DataContainer;
class Client;

// screen dimensions
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

// tile default width
extern int TILE_WIDTH;

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
extern LobbyScreen* lobbyScreen;
extern NotifyScreen* notifyScreen;

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
extern std::string username;

// client entity
extern Client* client;

#endif
