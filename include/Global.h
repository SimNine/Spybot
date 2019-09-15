#include <SDL.h>

#include "GUIContainer.h"
#include "DataContainer.h"
#include "MapScreen.h"
#include "GameScreen.h"

// screen dimensions
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

// window to render in
extern SDL_Window* gWindow;

// designate the renderer
extern SDL_Renderer* gRenderer;

// designate the GUI_containers
enum screen { SCREEN_TITLE, SCREEN_MAP, SCREEN_GAME };
extern screen scrn;
extern GUIContainer* titleScreen;
extern MapScreen* mapScreen;
extern GameScreen* gameScreen;

// container to hold pointers to node textures
extern DataContainer* dataContainer;

// mouse data
extern int mousePosX;
extern int mousePosY;
extern bool mousePressed;

// action flags
extern bool nextDraw;
extern bool nextInput;
extern bool nextTick;

// quit flag
extern bool quit;

// debug flag
extern bool debug;
