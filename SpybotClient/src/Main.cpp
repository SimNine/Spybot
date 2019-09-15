#include "Standard.h"
#include "Main.h"

#include "Global.h"
#include "TitleScreen.h"
#include "MainScreen.h"
#include "MapScreen.h"
#include "GameScreen.h"
#include "LobbyScreen.h"
#include "NotifyScreen.h"
#include "Data.h"
#include "Client.h"
#include "Timer.h"
#include "ChatDisplay.h"

void savePrefs();
void loadPrefs();

void saveProgs();
void loadProgs();

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
	_titleScreen->resetBounds();
	_mainScreen->resetBounds();
	_mapScreen->resetBounds();
	_gameScreen->resetBounds();
	_lobbyScreen->resetBounds();
	_notifyScreen->resetBounds();
}

// initializes the GUIs and switches the screen to titleScreen
void initGUIs() {
	_titleScreen = new TitleScreen();
	_mainScreen = new MainScreen();
	_mapScreen = new MapScreen();
	_mapScreen->switchMap(MAPPRESET_CLASSIC);
	_gameScreen = new GameScreen();
	_lobbyScreen = new LobbyScreen();
	_notifyScreen = new NotifyScreen();

	_currScreen = _titleScreen;
}

// initializes SDL and SDL_image
bool initSDL() {
	// set non-thread-naming hint
	if (!SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1")) {
		printf("Warning: thread non-naming failed to enable!");
	}

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Create window
	int flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
	_window = SDL_CreateWindow("Spybot: The Nightfall Incident", 50, 50, _SCREEN_WIDTH, _SCREEN_HEIGHT, flags);
	if (_window == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetWindowMinimumSize(_window, _SCREEN_WIDTH, _SCREEN_HEIGHT);
	SDL_ShowCursor(SDL_DISABLE);

	//Create renderer for window
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	// initialize TTF
	if (TTF_Init() != 0) {
		printf("SDL_TTF could not initialize! SDL_TTF error: %s\n", TTF_GetError());
		return false;
	}

	loadPrefs();
	loadProgs();
	initData();
	initGUIs(); // initialize GUIContainers
	resetBounds();

	return true;
}

// initialize winsock
bool initWinsock() {

	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	// initialize the client entity
	_client = new Client();

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

// save the user's preferences
void savePrefs() {
	std::ofstream prefs;
	prefs.open("default.prefs", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!prefs.is_open()) {
		if (_debug >= DEBUG_MINIMAL) printf("err saving prefs\n");
	} else {
		if (_debug >= DEBUG_MINIMAL) printf("saving prefs...\n");

		// begin by writing the sizes of various data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		if (_debug >= DEBUG_NORMAL) printf("saving constants... int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);
		prefs.write((char*)&sizeOfInt, 1);
		prefs.write((char*)&sizeOfChar, 1);
		prefs.write((char*)&sizeOfDouble, 1);
		prefs.write((char*)&sizeOfBool, 1);

		// save preferences
		int soundVol = Mix_Volume(-1, 0);
		int musicVol = Mix_VolumeMusic(0);
		Uint32 windowFlags = SDL_GetWindowFlags(_window);
		if (_debug >= DEBUG_NORMAL) printf("saving preferences... soundVol:%i, musicVol:%i, SCRWIDTH:%i, SCRHEIGHT:%i\n",
			soundVol, musicVol, _SCREEN_WIDTH, _SCREEN_HEIGHT);
		prefs.write((char*)&soundVol, sizeOfInt);
		prefs.write((char*)&musicVol, sizeOfInt);
		prefs.write((char*)&windowFlags, sizeof(Uint32));
		prefs.write((char*)&_SCREEN_WIDTH, sizeOfInt);
		prefs.write((char*)&_SCREEN_HEIGHT, sizeOfInt);

		// flush and close
		if (_debug >= DEBUG_MINIMAL) printf("flushing and closing prefs file... ");
		prefs.flush();
		prefs.close();
		if (_debug >= DEBUG_MINIMAL) printf("done\n");
	}
}

// load the user's preferences
void loadPrefs() {
	std::ifstream prefs;
	prefs.open("default.prefs", std::ios::in | std::ios::binary);

	if (!prefs.is_open()) {
		if (_debug >= DEBUG_MINIMAL) printf("err reading prefs\n");
	} else {
		if (_debug >= DEBUG_MINIMAL) printf("reading prefs...\n");

		// read the sizes of various data types
		if (_debug >= DEBUG_NORMAL) printf("loading constants...\n");
		int8_t sizeOfInt;
		prefs.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		prefs.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		prefs.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		prefs.read((char*)&sizeOfBool, 1);
		if (_debug >= DEBUG_NORMAL) printf("loaded constants: int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);

		// read prefs
		if (_debug >= DEBUG_NORMAL) printf("loading prefs...\n");
		int soundVol;
		prefs.read((char*)&soundVol, sizeOfInt);
		Mix_Volume(-1, soundVol);
		int musicVol;
		prefs.read((char*)&musicVol, sizeOfInt);
		Mix_VolumeMusic(musicVol);
		int windowFlags;
		prefs.read((char*)&windowFlags, sizeof(Uint32));
		if (windowFlags & SDL_WINDOW_FULLSCREEN) SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		prefs.read((char*)&_SCREEN_WIDTH, sizeOfInt);
		prefs.read((char*)&_SCREEN_HEIGHT, sizeOfInt);
		SDL_SetWindowSize(_window, _SCREEN_WIDTH, _SCREEN_HEIGHT);
		if (_debug >= DEBUG_NORMAL) printf("loaded prefs: soundVol:%i, musicVol:%i, fullscreen:%i, SCRWIDTH:%i, SCRHEIGHT:%i\n",
			soundVol, musicVol, windowFlags, _SCREEN_WIDTH, _SCREEN_HEIGHT);

		// close the file
		prefs.close();
		if (_debug >= DEBUG_MINIMAL) printf("done\n");
	}
}

// save the user's programs
void saveProgs() {
	std::ofstream progs;
	progs.open("default.progs", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!progs.is_open()) {
		if (_debug >= DEBUG_MINIMAL) printf("err saving programs\n");
	} else {
		if (_debug >= DEBUG_MINIMAL) printf("saving programs...\n");

		// save constants
		int8_t sizeOfInt = sizeof(int);
		progs.write((char*)&sizeOfInt, 1);

		// save programs
		for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
			progs.write((char*)&_progListClassic[i], sizeOfInt);
			progs.write((char*)&_progListNightfall[i], sizeOfInt);
			progs.write((char*)&_progListCustom[i], sizeOfInt);
		}

		// flush and close
		if (_debug >= DEBUG_MINIMAL) printf("flushing and closing programs file... ");
		progs.flush();
		progs.close();
		if (_debug >= DEBUG_MINIMAL) printf("done\n");
	}
}

// load the user's programs
void loadProgs() {
	std::ifstream progs;
	progs.open("default.progs", std::ios::in | std::ios::binary);

	if (!progs.is_open()) {
		if (_debug >= DEBUG_MINIMAL) printf("err reading programs\n");
	} else {
		if (_debug >= DEBUG_MINIMAL) printf("reading programs...\n");

		// load constants
		int8_t sizeOfInt;
		progs.read((char*)&sizeOfInt, 1);

		// load programs
		for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
			progs.read((char*)&_progListClassic[i], sizeOfInt);
			progs.read((char*)&_progListNightfall[i], sizeOfInt);
			progs.read((char*)&_progListCustom[i], sizeOfInt);
			_usedPrograms[i] = 0;
		}

		// close the file
		progs.close();
		if (_debug >= DEBUG_MINIMAL) printf("done\n");
	}
}

// handle all events in SDL event queue
void handleEvents() {
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0) {
		// handle events not specific to a screen
		if (e.type == SDL_QUIT) // window close
		{
			_quit = true;
		} else if (e.type == SDL_WINDOWEVENT) // window resize
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				_SCREEN_WIDTH = e.window.data1;
				_SCREEN_HEIGHT = e.window.data2;
				resetBounds();
			}
		} else if (e.type == SDL_KEYDOWN) // keypress
		{
			if (e.key.keysym.sym == SDLK_F12) // toggle fullscreen on F12
			{
				toggleFullscreen();
			} else if (e.key.keysym.sym == SDLK_F2) {
				if (_debug == DEBUG_EXTRA) _debug = DEBUG_NONE;
				else _debug = (DEBUG)(_debug + 1);
			} else if (e.key.keysym.sym == SDLK_p) {
				_progListCurrent[rand() % PROGRAM_NUM_PROGTYPES]++;
				_mapScreen->resetProgramInvDisplay();
			}
		} else if (e.type == SDL_MOUSEMOTION) {
			if (_heldContainer != NULL) {
				Coord delta = { e.motion.x - _mousePos.x, e.motion.y - _mousePos.y };
				if (_debug >= DEBUG_EXTRA) printf("%i,%i\n", delta.x, delta.y);
				_heldContainer->incDisplacement(delta);
			}

			_mousePos.x = e.motion.x;
			_mousePos.y = e.motion.y;
		} else if (e.type == SDL_MOUSEBUTTONDOWN) {
			_mousePressed = true;
			_currScreen->mouseDown();
		} else if (e.type == SDL_MOUSEBUTTONUP) {
			_mousePressed = false;
			_currScreen->mouseUp();
		}

		// screen-specific input
		if (_currScreen == _gameScreen) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_s) {
					_gameScreen->saveGame();
				} else if (e.key.keysym.sym == SDLK_ESCAPE) {
					_gameScreen->togglePauseMenu();
				} else if (e.key.keysym.sym == SDLK_F3) {
					_gameScreen->toggleEditorMode();
				}

				// handle chat display
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					_gameScreen->getChatDisplay()->addInputChar(out);
			}
		} else if (_currScreen == _mapScreen) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_l) {
					_mapScreen->unlockAllLevels();
				} else if (e.key.keysym.sym == SDLK_ESCAPE) {
					_mapScreen->togglePauseMenu();
				}
			}
		} else if (_currScreen == _mainScreen) {
			if (e.type == SDL_KEYDOWN) {
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					_mainScreen->keyPress(out);
			}
		} else if (_currScreen == _lobbyScreen) {
			if (e.type == SDL_KEYDOWN) {
				SDL_Keycode keycode = e.key.keysym.sym;
				bool isShiftPressed = (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT]);

				char out = convertSDLKeycodeToChar(keycode, isShiftPressed);
				if (out != 0)
					_lobbyScreen->getChatDisplay()->addInputChar(out);
			}
		}
	}
}

// draw the current screen
void draw() {
	_currScreen->draw();
	_notifyScreen->draw();
	SDL_RenderPresent(_renderer); // update the screen
}

// tick the current screen
void tick(int ms) {
	_currScreen->tick(ms);
	_notifyScreen->tick(ms);
}

// main function
int main(int argc, char* args[]) {
	// start up winsock and create the client
	if (!initWinsock()) {
		printf("Failed to initialize Winsock\n");
	}

	// start up SDL and create window
	if (!initSDL()) {
		printf("Failed to initialize SDL\n");
		return 1;
	}

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

		// handle ticks
		unsigned int ticksPassed = tickTimer.getTicks();
		if (tickCap == 0) {
			tickTimer.stop();
			tickTimer.start();
			tick(ticksPassed); // tick whatever needs to be ticked
			if (_acceptingInput) handleEvents();
			tickCount++;
		}
		if (ticksPassed >= msPerTick) { // tick whenever 20+ ms have gone by
			tickTimer.stop();
			tickTimer.start();
			tick(ticksPassed); // tick whatever needs to be ticked
			if (_acceptingInput) handleEvents();
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
				printf("avg fps: %f\n", fps);
				printf("num frames: %i\n\n", frameCount);
				frameCount = 0;
				renderTimer.start();
			}
		}
	}

	//Free resources, pointers, and close SDL
	savePrefs();
	saveProgs();
	closeSDL();

	return 0;
}
