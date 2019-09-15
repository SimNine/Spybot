#include "Standard.h"
#include "Global.h"

#include "GUIContainer.h"
#include "TitleScreen.h"
#include "MainScreen.h"
#include "MapScreen.h"
#include "GameScreen.h"
#include "LobbyScreen.h"
#include "NotifyScreen.h"
#include "Data.h"
#include "Client.h"
#include "GUITextbox.h"
#include "ConnectionManager.h"
#include "Server.h"

// initial screen dimension constants
int _SCREEN_WIDTH = 800;
int _SCREEN_HEIGHT = 600;

// default tile width
int _TILE_WIDTH = 32;

// default board dims
int _BOARD_WIDTH = 200;
int _BOARD_HEIGHT = 200;

// window to render to
SDL_Window* _window = NULL;

// designate the renderer
SDL_Renderer* _renderer = NULL;

// designate the GUI_containers
GUIContainer* _currScreen = NULL;
TitleScreen* _titleScreen = NULL;
MainScreen* _mainScreen = NULL;
MapScreen* _mapScreen = NULL;
GameScreen* _gameScreen = NULL;
LobbyScreen* _lobbyScreen = NULL;
NotifyScreen* _notifyScreen = NULL;

// mouse position
Coord _mousePos = { 0, 0 };
bool _mousePressed = false;

// update flags
bool _acceptingInput = true;

// quit flag
bool _quit = false;

// debug flag
DEBUG _debug = DEBUG_NONE;

// owned programs per campaign
int _progListClassic[PROGRAM_NUM_PROGTYPES];
int _progListNightfall[PROGRAM_NUM_PROGTYPES];
int _progListCustom[PROGRAM_NUM_PROGTYPES];
int _progListMulti[PROGRAM_NUM_PROGTYPES];
int* _progListCurrent = _progListClassic;

// programs currently in play
int _usedPrograms[PROGRAM_NUM_PROGTYPES];

// program ID currently selected (to be placed)
PROGRAM _currProgram = PROGRAM_NONE;

// GUIContainer currently held by the mouse
GUIContainer* _heldContainer = NULL;

// GUITextbox currently active
GUITextbox* _activeTextbox = NULL;

// credit counter
int _numCredits = 0;

// client monolith
Client* _client = NULL;

// client connection manager monolith
ConnectionManager* _connectionManager = NULL;

// server monolith
Server* _server = NULL;