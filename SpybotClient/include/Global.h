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
class Client;
class GUITextbox;
class ConnectionManager;
class Server;

// screen dimensions
extern int _SCREEN_WIDTH;
extern int _SCREEN_HEIGHT;

// tile default width
extern int _TILE_WIDTH;

// default game dims
extern int _BOARD_WIDTH;
extern int _BOARD_HEIGHT;

// window to render in
extern SDL_Window* _window;

// designate the renderer
extern SDL_Renderer* _renderer;

// designate the GUI_containers
extern GUIContainer* _currScreen;
extern TitleScreen* _titleScreen;
extern MainScreen* _mainScreen;
extern MapScreen* _mapScreen;
extern GameScreen* _gameScreen;
extern LobbyScreen* _lobbyScreen;
extern NotifyScreen* _notifyScreen;

// mouse data
extern Coord _mousePos;
extern bool _mousePressed;

// action flags
extern bool _acceptingInput;

// quit flag
extern bool _quit;

// debug level flag
extern DEBUG _debug;

// programs owned in each campaign
extern int _progListClassic[PROGRAM_NUM_PROGTYPES];
extern int _progListNightfall[PROGRAM_NUM_PROGTYPES];
extern int _progListCustom[PROGRAM_NUM_PROGTYPES];
extern int _progListMulti[PROGRAM_NUM_PROGTYPES];
extern int* _progListCurrent;

// programs currently in play
extern int _usedPrograms[PROGRAM_NUM_PROGTYPES];

// program ID currently selected (to be placed)
extern PROGRAM _currProgram;

// GUIContainer currently held by the mouse
extern GUIContainer* _heldContainer;

// GUITextbox currently active
extern GUITextbox* _activeTextbox;

// credit counter
extern int _numCredits;

// client entity
extern Client* _client;

// client connection manager monolith
extern ConnectionManager* _connectionManager;

// server monolith
extern Server* _server;

#endif
