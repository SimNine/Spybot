#pragma once

#include "Standard.h"
#include "LinkedList.h"

class GUIContainer;
class TitleOverlay;
class MainOverlay;
class MapOverlay;
class GameOverlay;
class LobbyOverlay;
class NotifyOverlay;
class Client;
class GUITextbox;
class ConnectionManager;
class Server;
class BackgroundOverlay;
class LocalLoginOverlay;

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
extern LinkedList<GUIContainer*>* _overlayStack;
extern TitleOverlay* _titleOverlay;
extern MainOverlay* _mainOverlay;
extern MapOverlay* _mapOverlay;
extern GameOverlay* _gameOverlay;
extern LobbyOverlay* _lobbyOverlay;
extern NotifyOverlay* _notifyOverlay;
extern BackgroundOverlay* _backgroundOverlay;
extern LocalLoginOverlay* _localLoginOverlay;

// mouse data
extern Coord _mousePos;
extern bool _mousePressed;

// action flags
extern bool _acceptingInput;

// quit flag
extern bool _quit;

// debug level flag
extern DEBUG _debug;

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