#include "Standard.h"
#include "Global.h"

#include "GUIContainer.h"
#include "TitleOverlay.h"
#include "MainOverlay.h"
#include "MapOverlay.h"
#include "GameOverlay.h"
#include "LobbyOverlay.h"
#include "NotifyOverlay.h"
#include "Data.h"
#include "Client.h"
#include "GUITextbox.h"
#include "ConnectionManager.h"
#include "Server.h"

/*
GLOBAL VARIABLES
*/

// default tile width
int _tileWidth = 32;

// default board dims
int _boardWidth = 200;
int _boardHeight = 200;

// window to render to
SDL_Window* _window = NULL;

// designate the renderer
SDL_Renderer* _renderer = NULL;

// designate the GUI_containers
LinkedList<GUIContainer*>* _overlayStack = new LinkedList<GUIContainer*>();
TitleOverlay* _titleOverlay = NULL;
MainOverlay* _mainOverlay = NULL;
MapOverlay* _mapOverlay = NULL;
GameOverlay* _gameOverlay = NULL;
LobbyOverlay* _lobbyOverlay = NULL;
NotifyOverlay* _notifyOverlay = NULL;
BackgroundOverlay* _backgroundOverlay = NULL;

// mouse position
Coord _mousePos = { 0, 0 };
bool _mousePressed = false;

// update flags
bool _acceptingInput = true;

// quit flag
bool _quit = false;

// debug flag
DEBUG _debug = DEBUG_NONE;

// GUIContainer currently held by the mouse, if any
GUIContainer* _heldContainer = NULL;

// GUITextbox currently active, if any
GUITextbox* _activeTextbox = NULL;

// client monolith
Client* _client = NULL;

// client connection manager monolith
ConnectionManager* _connectionManager = NULL;

// server monolith
Server* _server = NULL;

/*
PREFERENCES
*/

// initial (and minimum) screen dimension constants
int _screenWidth = 800;
int _screenHeight = 600;

/*
PROGRESS / STATS
*/

bool _progressAchievements[ACHIEVEMENT_NUM_ACHIEVEMENTS];
bool _progressNightfall = false;
bool _progressFreeform = false;
int _progressGamesPlayed = 0;
int _progressGamesWon = 0;
int _progressGamesLost = 0;
int _progressProgramsKilled = 0;
int _progressCreditsCollected = 0;
int _progressFreeformGamesWon = 0;
long _progressTotalSecondsPlayed = 0;
long _progressTotalSecondsPlayedThisSession = 0;
int _progressMSPlayed = 0; // this is a temporary variable while seconds are incremented