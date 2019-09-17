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
class TimedEvent;
class BackgroundOverlay;

/*
GLOBAL VARIABLES
*/

// tile default width
extern int _tileWidth;

// default game dims
extern int _boardWidth;
extern int _boardHeight;

// window to render in
extern SDL_Window* _window;

// designate the renderer
extern SDL_Renderer* _renderer;

// designate a list of timed events
extern LinkedList<TimedEvent*>* _timedEvents;

// designate the GUI_containers
extern LinkedList<GUIContainer*>* _overlayStack;
extern TitleOverlay* _titleOverlay;
extern MainOverlay* _mainOverlay;
extern MapOverlay* _mapOverlay;
extern GameOverlay* _gameOverlay;
extern LobbyOverlay* _lobbyOverlay;
extern BackgroundOverlay* _backgroundOverlay;

extern GUIContainer* _fadeOverlay;
extern NotifyOverlay* _notifyOverlay;

// mouse data
extern Coord _mousePos;
extern bool _mousePressed;

// action flags
extern bool _acceptingInput;

// quit flag
extern bool _quit;

// debug level flag
extern DEBUG _debug;

// GUIContainer currently held by the mouse
extern GUIContainer* _heldContainer;

// GUITextbox currently active
extern GUITextbox* _activeTextbox;

// client entity
extern Client* _client;

// client connection manager monolith
extern ConnectionManager* _connectionManager;

// server monolith
extern Server* _server;

/*
PREFERENCES
*/

// screen dimensions
extern int _screenWidth;
extern int _screenHeight;

/*
PROGRESS
*/

extern bool _progressAchievements[ACHIEVEMENT_NUM_ACHIEVEMENTS];
extern bool _progressNightfall;
extern bool _progressFreeform;
extern int _progressGamesPlayed;
extern int _progressGamesWon;
extern int _progressGamesLost;
extern int _progressProgramsKilled;
extern int _progressCreditsCollected;
extern int _progressFreeformGamesWon;
extern long _progressTotalSecondsPlayed;
extern long _progressTotalSecondsPlayedThisSession;
extern int _progressMSPlayed; // this is a temporary variable while seconds are incremented