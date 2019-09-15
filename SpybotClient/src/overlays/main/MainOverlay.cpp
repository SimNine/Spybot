#include "Standard.h"
#include "MainOverlay.h"

#include "Global.h"
#include "GUITexture.h"
#include "Data.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "Main.h"
#include "Client.h"
#include "MapOverlay.h"
#include "LobbyOverlay.h"
#include "GlowSpeck.h"
#include "PromptBox.h"
#include "Message.h"
#include "GUITextbox.h"
#include "ConnectionManager.h"
#include "LocalLoginOverlay.h"
#include "BackgroundOverlay.h"

MainOverlay::MainOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _SCREEN_WIDTH, _SCREEN_HEIGHT }, NULL) {
	GUITexture* main_title = new GUITexture(this, ANCHOR_NORTHWEST, { 20, 20 }, { 499, 42 }, _title_title);
	addObject(main_title);
	GUITexture* main_subtitle = new GUITexture(this, ANCHOR_NORTHWEST, { 20, 82 }, { 390, 10 }, _title_subtitle, this);
	addObject(main_subtitle);

	// options container // TODO: FIX ANCHORS
	optionsContainer_ = new GUIContainer(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 1000, 500 }, NULL);
	GUIButton* options_backbutton = new GUIButton(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -20 }, "BACK", 50, [] () {_mainOverlay->toggleOptions(); });
	optionsContainer_->addObject(options_backbutton);
	GUISlider* options_slider_sound = new GUISlider(optionsContainer_, ANCHOR_SOUTHWEST, { 270, -150 }, { 200, 50 },
		[] (float d) { Mix_Volume(-1, (int)(d * 128)); });
	optionsContainer_->addObject(options_slider_sound);
	GUITexture* options_label_soundslider = new GUITexture(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -150 }, "Sound Volume:", 50);
	optionsContainer_->addObject(options_label_soundslider);
	GUISlider* options_slider_music = new GUISlider(optionsContainer_, ANCHOR_SOUTHWEST, { 270, -210 }, { 200, 50 },
		[] (float d) { Mix_VolumeMusic((int)(d * 128)); });
	optionsContainer_->addObject(options_slider_music);
	GUITexture* options_label_musicslider = new GUITexture(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -220 }, "Music Volume:", 50);
	optionsContainer_->addObject(options_label_musicslider);
	GUIButton* options_fullscreenbutton = new GUIButton(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -280 }, "Toggle Fullscreen", 50,
		[] () {toggleFullscreen(); });
	optionsContainer_->addObject(options_fullscreenbutton);
	addObject(optionsContainer_);

	optionsContainer_->setMovable(false);
	optionsContainer_->setTransparency(0);

	// main container
	mainContainer_ = new GUIContainer(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 1000, 500 }, NULL);
	int ln = 0;

	GUIButton* button_quit = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "QUIT", 50,
		[] () {
		Mix_PlayChannel(-1, _sound_move_player, 0);
		_quit = true;
	});
	mainContainer_->addObject(button_quit);

	GUIButton* button_options = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "OPTIONS", 50,
		[] () {
		_mainOverlay->toggleOptions();
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_options);

	GUIButton* button_achievements = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "ACHIEVEMENTS", 50, [] () {
		printf("placeholder: goto achievs");
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_achievements);

	GUIButton* button_multiplayer = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "MULTIPLAYER", 50,
		[] () {
		_mainOverlay->showIPEntry();
		_mainOverlay->hideMainContainer();
	});
	mainContainer_->addObject(button_multiplayer);

	GUIButton* button_freeform = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "FREEFORM HACKING", 50,
		[] () {
		printf("placeholder: goto freeform map");
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_freeform);

	GUIButton* button_nightfall = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "NIGHTFALL CAMPAIGN", 50,
		[] () {
		_connectionManager->connectToLocalServer("levels/nightfall");
		_mapOverlay->switchMap(MAPPRESET_NIGHTFALL);
		_overlayStack->removeAll();
		_overlayStack->push(_backgroundOverlay);
		_overlayStack->push(_localLoginOverlay);
		_localLoginOverlay->refreshUsers();
		Mix_PlayMusic(_music_map_ambient, -1);
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_nightfall);

	GUIButton* button_classic = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "CLASSIC CAMPAIGN", 50,
		[] () {
		_connectionManager->connectToLocalServer("levels/classic");
		_mapOverlay->switchMap(MAPPRESET_CLASSIC);
		_overlayStack->removeAll();
		_overlayStack->push(_backgroundOverlay);
		_overlayStack->push(_localLoginOverlay);
		_localLoginOverlay->refreshUsers();
		Mix_PlayMusic(_music_map_ambient, -1);
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_classic);

	addObject(mainContainer_);
	mainContainer_->setMovable(false);

	// IP prompt
	IPEntryContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 10000, 10000 }, NULL);
	IPEntryBox_ = new PromptBox(ANCHOR_CENTER, { 0, 0 }, { 600, 200 }, this, "Enter IP of server to connect to:",
		[] () {_mainOverlay->submitIPEntry(); _mainOverlay->hideMainContainer(); },
		[] () {_mainOverlay->hideIPEntry(); _mainOverlay->showMainContainer(); });
	IPEntryContainer_->addObject(IPEntryBox_);
	IPEntryContainer_->setTransparency(0);
	IPEntryContainer_->setMovable(false);
	addObject(IPEntryContainer_);

	// login prompt
	loginEntryContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 800, 400 }, _color_bkg_standard);
	GUITexture* usernamePrompt = new GUITexture(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 20 }, "Username:", 50);
	loginEntryContainer_->addObject(usernamePrompt);
	loginUsername_ = new GUITextbox(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 80 }, { 760, 40 }, DEFAULT_MSG_TEXTSIZE / 2 - 5, false);
	loginEntryContainer_->addObject(loginUsername_);
	GUITexture* passwordPrompt = new GUITexture(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 140 }, "Password:", 50);
	loginEntryContainer_->addObject(passwordPrompt);
	loginPassword_ = new GUITextbox(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 200 }, { 760, 40 }, DEFAULT_MSG_TEXTSIZE / 2 - 5, true);
	loginEntryContainer_->addObject(loginPassword_);
	loginCancelButton_ = new GUIButton(loginEntryContainer_, ANCHOR_SOUTHWEST, { 20, -20 }, "Cancel", 50, [] () {_mainOverlay->loginHide(); _connectionManager->disconnect(); });
	loginEntryContainer_->addObject(loginCancelButton_);
	loginCreateButton_ = new GUIButton(loginEntryContainer_, ANCHOR_SOUTH, { 0, -20 }, "Create Account", 50, [] () {_mainOverlay->loginCreate(); });
	loginEntryContainer_->addObject(loginCreateButton_);
	loginSubmitButton_ = new GUIButton(loginEntryContainer_, ANCHOR_SOUTHEAST, { -20, -20 }, "Login", 50, [] () {_mainOverlay->loginSubmit(); });
	loginEntryContainer_->addObject(loginSubmitButton_);
	loginEntryContainer_->setTransparency(0);
	addObject(loginEntryContainer_);
}

MainOverlay::~MainOverlay() {
	//dtor
}

void MainOverlay::draw() {
	// draw GUIObjects
	drawContents();
}

void MainOverlay::tick(int ms) {
	// tick all GUIObjects
	GUIContainer::tick(ms);
}

void MainOverlay::toggleOptions() {
	if (optionsContainer_->isVisible()) {
		optionsContainer_->fade(0, 1000);
		mainContainer_->fade(255, 1000);
	} else {
		optionsContainer_->fade(255, 1000);
		mainContainer_->fade(0, 1000);
	}
}

void MainOverlay::showIPEntry() {
	IPEntryContainer_->setTransparency(255);
	IPEntryBox_->clearContents();
	_activeTextbox = IPEntryBox_->getTextbox();
}

void MainOverlay::hideIPEntry() {
	IPEntryContainer_->setTransparency(0);
}

void MainOverlay::submitIPEntry() {
	_connectionManager->connectToExternalServer(IPEntryBox_->getContents());
}

void MainOverlay::loginShow() {
	loginEntryContainer_->setTransparency(255);
	loginUsername_->clearContents();
	loginPassword_->clearContents();
	_activeTextbox = loginUsername_;
}

void MainOverlay::loginHide() {
	loginEntryContainer_->setTransparency(0);
}

void MainOverlay::loginCreate() {
	std::string text = loginUsername_->getContents() + "\n" + loginPassword_->getContents();

	Message m;
	m.type = MSGTYPE_CREATEUSER;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, text.c_str(), DEFAULT_MSG_TEXTSIZE);
	_connectionManager->sendMessage(m);
}

void MainOverlay::loginSubmit() {
	std::string text = loginUsername_->getContents() + "\n" + loginPassword_->getContents();

	Message m;
	m.type = MSGTYPE_LOGIN;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, text.c_str(), DEFAULT_MSG_TEXTSIZE);
	_connectionManager->sendMessage(m);
}

void MainOverlay::loginClear() {
	loginUsername_->clearContents();
	loginPassword_->clearContents();
	_activeTextbox = loginUsername_;
}

void MainOverlay::hideMainContainer() {
	mainContainer_->setTransparency(0);
}

void MainOverlay::showMainContainer() {
	mainContainer_->setTransparency(255);
}

void MainOverlay::keyPress(char c) {
	if (IPEntryContainer_->isVisible())
		IPEntryBox_->addChar(c);
	if (loginEntryContainer_->isVisible()) {
		if (c == 13) { // return
			loginSubmitButton_->mouseDown();
			loginSubmitButton_->mouseUp();
		} else if (c == 27) { // escape
			loginCancelButton_->mouseDown();
			loginCancelButton_->mouseUp();
		} else if (c == 9) { // tab
			if (_activeTextbox == loginUsername_)
				_activeTextbox = loginPassword_;
			else if (_activeTextbox == loginPassword_)
				_activeTextbox = loginUsername_;
		} else if (loginUsername_ == _activeTextbox)
			loginUsername_->addChar(c);
		else if (loginPassword_ == _activeTextbox)
			loginPassword_->addChar(c);
	}
}