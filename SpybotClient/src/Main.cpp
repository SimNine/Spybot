#include "Standard.h"
#include "Main.h"

#include "Global.h"
#include "TitleOverlay.h"
#include "MainOverlay.h"
#include "MapOverlay.h"
#include "GameOverlay.h"
#include "LobbyOverlay.h"
#include "NotifyOverlay.h"
#include "Data.h"
#include "Client.h"
#include "Timer.h"
#include "ChatDisplay.h"
#include "ConnectionManager.h"
#include "Server.h"
#include "LinkedList.h"
#include "BackgroundOverlay.h"
#include "Message.h"
#include "TimedEvent.h"

void savePrefs();
void loadPrefs();

// converts an SDL_Keycode into a char
char convertSDLKeycodeToChar(SDL_Keycode keycode, bool shift) {
	const char* k = SDL_GetKeyName(keycode);
	if (strlen(k) == 1) {
		// get the key
		char key = k[0];

		// if alphabetical
		if (key >= 60 && key <= 90)
			return shift ? key : key + 32;

		// other regular chars
		switch (key) {
		case '1':
			return shift ? '!' : '1';
		case '2':
			return shift ? '@' : '2';
		case '3':
			return shift ? '#' : '3';
		case '4':
			return shift ? '$' : '4';
		case '5':
			return shift ? '%' : '5';
		case '6':
			return shift ? '^' : '6';
		case '7':
			return shift ? '&' : '7';
		case '8':
			return shift ? '*' : '8';
		case '9':
			return shift ? '(' : '9';
		case '0':
			return shift ? ')' : '0';
		case '/':
			return shift ? '?' : '/';
		case '.':
			return shift ? '>' : '.';
		case ',':
			return shift ? '<' : ',';
		case '-':
			return shift ? '_' : '-';
		case '=':
			return shift ? '+' : '=';
		case '\'':
			return shift ? '\"' : '\'';
		case ';':
			return shift ? ':' : ';';
		default:
			return 0;
		}
	} else if (keycode == SDLK_SPACE)
		return ' ';
	else if (keycode == SDLK_BACKSPACE)
		return 127;
	else if (keycode == SDLK_RETURN)
		return 13;
	else if (keycode == SDLK_ESCAPE)
		return 27;
	else if (keycode == SDLK_TAB)
		return 9;
	else
		return 0;
}

// closes SDL and SDL_image
void closeSDL() {
	//Destroy window
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	_window = NULL;
	_renderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// resets all GUI bounds
void resetBounds() {
	_titleOverlay->resetBounds();
	_mainOverlay->resetBounds();
	_mapOverlay->resetBounds();
	_gameOverlay->resetBounds();
	_lobbyOverlay->resetBounds();
	_backgroundOverlay->resetBounds();

	_fadeOverlay->resetBounds();
	_notifyOverlay->resetBounds();
}

// initializes the GUIs and switches the screen to titleScreen
void initGUIs() {
	_titleOverlay = new TitleOverlay();
	_mainOverlay = new MainOverlay();
	_mapOverlay = new MapOverlay();
	_gameOverlay = new GameOverlay();
	_lobbyOverlay = new LobbyOverlay();
	_backgroundOverlay = new BackgroundOverlay();

	_fadeOverlay = new GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenWidth }, _color_black);
	_notifyOverlay = new NotifyOverlay();

	_overlayStack->push(_titleOverlay);
}

// initializes SDL and SDL_image
bool initSDL() {
	// set non-thread-naming hint
	if (!SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1")) {
		log("Warning: thread non-naming failed to enable!");
	}

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		log("SDL could not initialize! SDL Error: " + std::string(SDL_GetError()) + "\n");
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		log("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	int flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
	_window = SDL_CreateWindow("Spybot: The Nightfall Incident", 50, 50, _screenWidth, _screenHeight, flags);
	if (_window == NULL) {
		log("Window could not be created! SDL Error: " + std::string(SDL_GetError()) + "\n");
		return false;
	}
	SDL_SetWindowMinimumSize(_window, _screenWidth, _screenHeight);
	SDL_ShowCursor(SDL_DISABLE);

	//Create renderer for window
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL) {
		log("Renderer could not be created! SDL Error: " + std::string(SDL_GetError()) + "\n");
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		log("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()) + "\n");
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		log("SDL_mixer could not initialize! SDL_mixer Error: " + std::string(Mix_GetError()) + "\n");
		return false;
	}

	// initialize TTF
	if (TTF_Init() != 0) {
		log("SDL_TTF could not initialize! SDL_TTF error: " + std::string(TTF_GetError()) + "\n");
		return false;
	}

	return true;
}

// initialize winsock
bool initWinsock() {
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		log("WSAStartup failed with error: " + to_string(iResult) + "\n");
		return false;
	}

	return true;
}

// toggle fullscreen on or off
void toggleFullscreen() {
	if ((SDL_GetWindowFlags(_window) & SDL_WINDOW_FULLSCREEN) > 0) {
		SDL_SetWindowFullscreen(_window, 0);
	} else {
		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	resetBounds();
}

// save the user's progress
void saveProgress() {
	std::ofstream prog;
	prog.open("progress.dat", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!prog.is_open()) {
		if (_debug >= DEBUG_MINIMAL)
			log("err saving progress\n");
	} else {
		if (_debug >= DEBUG_MINIMAL)
			log("saving progress...\n");

		// write the size of various data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		int8_t sizeOfLong = sizeof(long);
		if (_debug >= DEBUG_NORMAL)
			log("saving constants... int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) + " double:" + to_string(sizeOfDouble) +
				" bool:" + to_string(sizeOfBool) + " long:" + to_string(sizeOfLong) + "\n");
		prog.write((char*)&sizeOfInt, 1);
		prog.write((char*)&sizeOfChar, 1);
		prog.write((char*)&sizeOfDouble, 1);
		prog.write((char*)&sizeOfBool, 1);
		prog.write((char*)&sizeOfLong, 1);

		// save progress
		prog.write((char*)&_progressNightfall, sizeOfBool);
		prog.write((char*)&_progressFreeform, sizeOfBool);
		for (int i = 0; i < ACHIEVEMENT_NUM_ACHIEVEMENTS; i++) {
			prog.write((char*)&_progressAchievements[i], sizeOfBool);
		}
		prog.write((char*)&_progressGamesPlayed, sizeOfInt);
		prog.write((char*)&_progressGamesWon, sizeOfInt);
		prog.write((char*)&_progressGamesLost, sizeOfInt);
		prog.write((char*)&_progressProgramsKilled, sizeOfInt);
		prog.write((char*)&_progressCreditsCollected, sizeOfInt);
		prog.write((char*)&_progressFreeformGamesWon, sizeOfInt);
		prog.write((char*)&_progressTotalSecondsPlayed, sizeOfLong);

		// flush and close
		if (_debug >= DEBUG_MINIMAL)
			log("flushing and closing prefs file... ");
		prog.flush();
		prog.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");
	}
}

// load the user's progress
void loadProgress() {
	std::ifstream prog;
	prog.open("progress.dat", std::ios::in | std::ios::binary);

	if (!prog.is_open()) {
		if (_debug >= DEBUG_MINIMAL)
			log("err reading progress\n");
	} else {
		if (_debug >= DEBUG_MINIMAL)
			log("reading progress...\n");

		// read the sizes of various data types
		if (_debug >= DEBUG_NORMAL)
			log("loading constants...\n");
		int8_t sizeOfInt;
		prog.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		prog.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		prog.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		prog.read((char*)&sizeOfBool, 1);
		int8_t sizeOfLong;
		prog.read((char*)&sizeOfLong, 1);
		if (_debug >= DEBUG_NORMAL)
			log("loaded constants: int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) + " double:" + to_string(sizeOfDouble) + 
				" bool:" + to_string(sizeOfBool) + " long:" + to_string(sizeOfLong) + "\n");

		// read prefs
		if (_debug >= DEBUG_NORMAL)
			log("loading progress...\n");
		prog.read((char*)&_progressNightfall, sizeOfBool);
		prog.read((char*)&_progressFreeform, sizeOfBool);
		for (int i = 0; i < ACHIEVEMENT_NUM_ACHIEVEMENTS; i++) {
			prog.read((char*)&_progressAchievements[i], sizeOfBool);
		}
		prog.read((char*)&_progressGamesPlayed, sizeOfInt);
		prog.read((char*)&_progressGamesWon, sizeOfInt);
		prog.read((char*)&_progressGamesLost, sizeOfInt);
		prog.read((char*)&_progressProgramsKilled, sizeOfInt);
		prog.read((char*)&_progressCreditsCollected, sizeOfInt);
		prog.read((char*)&_progressFreeformGamesWon, sizeOfInt);
		prog.read((char*)&_progressTotalSecondsPlayed, sizeOfLong);

		// close the file
		prog.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");
	}
}

// save the user's preferences
void savePrefs() {
	std::ofstream prefs;
	prefs.open("prefs.dat", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!prefs.is_open()) {
		if (_debug >= DEBUG_MINIMAL)
			log("err saving prefs\n");
	} else {
		if (_debug >= DEBUG_MINIMAL)
			log("saving prefs...\n");

		// begin by writing the sizes of various data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		int8_t sizeOfLong = sizeof(long);
		if (_debug >= DEBUG_NORMAL)
			log("saving constants: int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) + " double:" + to_string(sizeOfDouble) +
				" bool:" + to_string(sizeOfBool) + " long:" + to_string(sizeOfLong) + "\n");
		prefs.write((char*)&sizeOfInt, 1);
		prefs.write((char*)&sizeOfChar, 1);
		prefs.write((char*)&sizeOfDouble, 1);
		prefs.write((char*)&sizeOfBool, 1);
		prefs.write((char*)&sizeOfLong, 1);

		// save preferences
		int soundVol = Mix_Volume(-1, 0);
		int musicVol = Mix_VolumeMusic(0);
		Uint32 windowFlags = SDL_GetWindowFlags(_window);
		if (_debug >= DEBUG_NORMAL)
			log("saving preferences... soundVol:" + to_string(soundVol) + " musicVol:" + to_string(musicVol) + 
				" SCRWIDTH:" + to_string(_screenWidth) + " SCRHEIGHT:" + to_string(_screenHeight) + "\n");
		prefs.write((char*)&soundVol, sizeOfInt);
		prefs.write((char*)&musicVol, sizeOfInt);
		prefs.write((char*)&windowFlags, sizeof(Uint32));
		prefs.write((char*)&_screenWidth, sizeOfInt);
		prefs.write((char*)&_screenHeight, sizeOfInt);

		// flush and close
		if (_debug >= DEBUG_MINIMAL)
			log("flushing and closing prefs file... ");
		prefs.flush();
		prefs.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");
	}
}

// load the user's preferences
void loadPrefs() {
	std::ifstream prefs;
	prefs.open("prefs.dat", std::ios::in | std::ios::binary);

	if (!prefs.is_open()) {
		if (_debug >= DEBUG_MINIMAL)
			log("err reading prefs\n");
	} else {
		if (_debug >= DEBUG_MINIMAL)
			log("reading prefs...\n");

		// read the sizes of various data types
		if (_debug >= DEBUG_NORMAL)
			log("loading constants...\n");
		int8_t sizeOfInt;
		prefs.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		prefs.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		prefs.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		prefs.read((char*)&sizeOfBool, 1);
		int8_t sizeOfLong;
		prefs.read((char*)&sizeOfLong, 1);
		if (_debug >= DEBUG_NORMAL)
			log("loaded constants: int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) + " double:" + to_string(sizeOfDouble) +
				" bool:" + to_string(sizeOfBool) + " long:" + to_string(sizeOfLong) + "\n");

		// read prefs
		if (_debug >= DEBUG_NORMAL)
			log("loading prefs...\n");
		int soundVol;
		prefs.read((char*)&soundVol, sizeOfInt);
		Mix_Volume(-1, soundVol);
		int musicVol;
		prefs.read((char*)&musicVol, sizeOfInt);
		Mix_VolumeMusic(musicVol);
		int windowFlags;
		prefs.read((char*)&windowFlags, sizeof(Uint32));
		if (windowFlags & SDL_WINDOW_FULLSCREEN) 
			SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		prefs.read((char*)&_screenWidth, sizeOfInt);
		prefs.read((char*)&_screenHeight, sizeOfInt);
		SDL_SetWindowSize(_window, _screenWidth, _screenHeight);
		if (_debug >= DEBUG_NORMAL)
			log("loaded preferences: soundVol:" + to_string(soundVol) + " musicVol:" + to_string(musicVol) +
				" SCRWIDTH:" + to_string(_screenWidth) + " SCRHEIGHT:" + to_string(_screenHeight) + "\n");

		// close the file
		prefs.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");
	}
}

// handle all events in SDL event queue
void handleEvents() {
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0) {
		// handle events not specific to a screen
		if (e.type == SDL_QUIT) { // window close
			_quit = true;
		} else if (e.type == SDL_WINDOWEVENT) { // window resize
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				_screenWidth = e.window.data1;
				_screenHeight = e.window.data2;
				resetBounds();
			}
		} else if (e.type == SDL_KEYDOWN) { // keypress
			if (e.key.keysym.sym == SDLK_F12) { // toggle fullscreen on F12
				toggleFullscreen();
			} else if (e.key.keysym.sym == SDLK_F2) {
				if (_debug == DEBUG_EXTRA) _debug = DEBUG_NONE;
				else _debug = (DEBUG)(_debug + 1);
			}
		} else if (e.type == SDL_MOUSEMOTION) {
			if (_heldContainer != NULL) {
				Coord delta = { e.motion.x - _mousePos.x, e.motion.y - _mousePos.y };
				if (_debug >= DEBUG_EXTRA) 
					log(to_string(delta.x) + "," + to_string(delta.y) + "\n");
				_heldContainer->incDisplacement(delta);
			}

			_mousePos.x = e.motion.x;
			_mousePos.y = e.motion.y;
		} else if (e.type == SDL_MOUSEBUTTONDOWN) {
			_mousePressed = true;
			_overlayStack->getFirst()->mouseDown();
		} else if (e.type == SDL_MOUSEBUTTONUP) {
			_mousePressed = false;
			_overlayStack->getFirst()->mouseUp();
		}

		// screen-specific input
		if (_overlayStack->getFirst() == _gameOverlay) {
			if (e.type == SDL_KEYDOWN) {
				// show or hide pause menu
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					_gameOverlay->showPauseMenu();
				}

				// handle chat display
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					_gameOverlay->getChatDisplay()->addInputChar(out);
			}
		} else if (_overlayStack->getFirst() == _mapOverlay) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_l) {
					_mapOverlay->unlockAllLevels();
				} else if (e.key.keysym.sym == SDLK_ESCAPE) {
					_mapOverlay->showPauseMenu();
				} else if (e.key.keysym.sym == SDLK_p) {
					Message m;
					m.type = MSGTYPE_PROGINVENTORY;
					m.progType = (PROGRAM)(rand() % PROGRAM_NUM_PROGTYPES);
					m.num = 1;
					m.clientID = _client->getClientID();
					_connectionManager->sendMessage(m);
				}
			}
		} else if (_overlayStack->getFirst() == _mainOverlay) {
			if (e.type == SDL_KEYDOWN) {
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					_mainOverlay->keyPress(out);
			}
		} else if (_overlayStack->getFirst() == _lobbyOverlay) {
			if (e.type == SDL_KEYDOWN) {
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					_lobbyOverlay->getChatDisplay()->addInputChar(out);
			}
		}
	}
}

// draw each overlay in the stack
void draw() {
	for (int i = _overlayStack->getLength() - 1; i >= 0; i--) {
		_overlayStack->getObjectAt(i)->draw();
	}
	_fadeOverlay->draw();
	_notifyOverlay->draw();

	SDL_RenderPresent(_renderer); // update the screen
}

// tick each screen in the stack and check for time-related achievements
void tick(int ms) {
	// tick every screen in the stack
	for (int i = _overlayStack->getLength() - 1; i >= 0; i--) {
		_overlayStack->getObjectAt(i)->tick(ms);
	}
	_fadeOverlay->tick(ms);
	_notifyOverlay->tick(ms);

	// tick all overlaystackeffects
	Iterator<TimedEvent*> oStackIt = _timedEvents->getIterator();
	LinkedList<TimedEvent*> toRemove;
	while (oStackIt.hasNext()) {
		TimedEvent* curr = oStackIt.next();
		curr->tick(ms);
		if (curr->isDone()) {
			curr->execute();
			toRemove.addFirst(curr);
		}
	}

	// remove all dead overlaystackeffects
	Iterator<TimedEvent*> deadStackEventIt = toRemove.getIterator();
	while (deadStackEventIt.hasNext()) {
		TimedEvent* curr = deadStackEventIt.next();
		_timedEvents->remove(curr);
		delete curr;
	}

	// check for time-based achievements
	_progressMSPlayed += ms;
	if (_progressMSPlayed > 1000) {
		int numSeconds = _progressMSPlayed / 1000;
		_progressMSPlayed -= numSeconds * 1000;
		_progressTotalSecondsPlayed += numSeconds;
		_progressTotalSecondsPlayedThisSession += numSeconds;

		if (!_progressAchievements[ACHIEVEMENT_PARTICIPATION]) {
			unlockAchievement(ACHIEVEMENT_PARTICIPATION);
		} else if (!_progressAchievements[ACHIEVEMENT_ONEMINUTE] && _progressTotalSecondsPlayed >= 60) {
			unlockAchievement(ACHIEVEMENT_ONEMINUTE);
		} else if (!_progressAchievements[ACHIEVEMENT_FIVEMINUTES] && _progressTotalSecondsPlayed >= 60 * 5) {
			unlockAchievement(ACHIEVEMENT_FIVEMINUTES);
		} else if (!_progressAchievements[ACHIEVEMENT_ONEHOUR] && _progressTotalSecondsPlayed >= 60 * 60) {
			unlockAchievement(ACHIEVEMENT_ONEHOUR);
		} else if (!_progressAchievements[ACHIEVEMENT_FIVEHOURS] && _progressTotalSecondsPlayed >= 60 * 60 * 5) {
			unlockAchievement(ACHIEVEMENT_FIVEHOURS);
		} else if (!_progressAchievements[ACHIEVEMENT_TENHOURS] && _progressTotalSecondsPlayed >= 60 * 60 * 10) {
			unlockAchievement(ACHIEVEMENT_TENHOURS);
		} else if (!_progressAchievements[ACHIEVEMENT_FIFTYHOURS] && _progressTotalSecondsPlayed >= 60 * 60 * 50) {
			unlockAchievement(ACHIEVEMENT_FIFTYHOURS);
		} else if (!_progressAchievements[ACHIEVEMENT_CENTENNIAL] && _progressTotalSecondsPlayed >= 60 * 60 * 100) {
			unlockAchievement(ACHIEVEMENT_CENTENNIAL);
		}

		if (!_progressAchievements[ACHIEVEMENT_CASUALGAMER] && _progressTotalSecondsPlayedThisSession >= 60 * 5) {
			unlockAchievement(ACHIEVEMENT_CASUALGAMER);
		} else if (!_progressAchievements[ACHIEVEMENT_SERIOUSCASUALGAMER] && _progressTotalSecondsPlayedThisSession >= 60 * 30) {
			unlockAchievement(ACHIEVEMENT_SERIOUSCASUALGAMER);
		} else if (!_progressAchievements[ACHIEVEMENT_GAMER] && _progressTotalSecondsPlayedThisSession >= 60 * 60) {
			unlockAchievement(ACHIEVEMENT_GAMER);
		} else if (!_progressAchievements[ACHIEVEMENT_DEDICATEDGAMER] && _progressTotalSecondsPlayedThisSession >= 60 * 60 * 3) {
			unlockAchievement(ACHIEVEMENT_DEDICATEDGAMER);
		} else if (!_progressAchievements[ACHIEVEMENT_TAKEABREAK] && _progressTotalSecondsPlayedThisSession >= 60 * 60 * 5) {
			unlockAchievement(ACHIEVEMENT_TAKEABREAK);
		} else if (!_progressAchievements[ACHIEVEMENT_TIMETOSTOP] && _progressTotalSecondsPlayedThisSession >= 60 * 60 * 7) {
			unlockAchievement(ACHIEVEMENT_TIMETOSTOP);
		} else if (!_progressAchievements[ACHIEVEMENT_WHY] && _progressTotalSecondsPlayedThisSession >= 60 * 60 * 10) {
			unlockAchievement(ACHIEVEMENT_WHY);
		}
	}
}

// convenience method to do everything necessary when unlocking an achievement
void unlockAchievement(ACHIEVEMENT a) {
	_progressAchievements[a] = true;
	_notifyOverlay->addAchievement(a);
	saveProgress();
}

// main function
int main(int argc, char* args[]) {
	// log app entry
	logInit("log-client.txt", false);
	log("\n");
	log("-----SpybotClient-----\n");

	// start up winsock
	if (!initWinsock()) {
		log("Failed to initialize Winsock\n");
		return 1;
	}

	// start up SDL and create window
	if (!initSDL()) {
		log("Failed to initialize SDL\n");
		return 1;
	}

	// initialize connectionmanager and client
	_client = new Client();
	_connectionManager = new ConnectionManager();

	loadProgress(); // load progress
	loadPrefs(); // load GUI preferences
	initData(); // initialize resource data
	initGUIs(); // initialize GUIContainers
	resetBounds(); // reset bounds of all containers
	_fadeOverlay->addEffect(new GUIEffectFade(0, 100, 255, 0));

	// setup timers
	Timer tickTimer;
	tickTimer.start();
	Timer renderTimer;
	renderTimer.start();

	unsigned int fpsCap = 60;
	unsigned int tickCap = 60;
	unsigned int frameCount = 0;
	unsigned int tickCount = 0;
	unsigned int msPerFrame;
	unsigned int msPerTick;
	Timer renderTimerB;
	Timer tickTimerB;
	if (fpsCap != 0) {
		renderTimerB.start();
		msPerFrame = 1000 / fpsCap;
	}
	if (tickCap != 0) {
		tickTimerB.start();
		msPerTick = 1000 / tickCap;
	}

	// main loop
	SDL_ShowWindow(_window);
	while (!_quit) {
		// process messages
		_client->processAllMessages();
		if (_server != NULL) {
			_server->processAllMessages();
		}

		// handle ticks
		unsigned int ticksPassed = tickTimer.getTicks();
		if (tickCap == 0) {
			tickTimer.stop();
			tickTimer.start();
			tick(ticksPassed); // tick whatever needs to be ticked
			if (_acceptingInput)
				handleEvents();
			tickCount++;
		}
		if (ticksPassed >= msPerTick) { // tick whenever 20+ ms have gone by
			tickTimer.stop();
			tickTimer.start();
			tick(ticksPassed); // tick whatever needs to be ticked
			if (_acceptingInput)
				handleEvents();
			tickCount++;
		}

		// handle drawing
		if (fpsCap == 0) { // if there is no fps cap
			draw(); // render the window
			frameCount++;
		} else if (renderTimerB.getTicks() >= msPerFrame) { // if there is an fps cap and enough ms have gone
			draw();
			frameCount++;
			renderTimerB.stop();
			renderTimerB.start();
		}

		if (_debug >= DEBUG_EXTRA) {
			unsigned int numTicks = renderTimer.getTicks();
			if (numTicks >= 1000) {
				float fps = (float)(frameCount / (numTicks / 1000.0));
				renderTimer.stop();
				log("avg fps: " + to_string((int)fps) + "\n");
				log("num frames: " + to_string(frameCount) + "\n\n");
				frameCount = 0;
				renderTimer.start();
			}
		}
	}

	// save preferences and close SDL
	saveProgress();
	savePrefs();
	closeSDL();

	return 0;
}
