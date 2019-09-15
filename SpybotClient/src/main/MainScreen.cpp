#include "Standard.h"
#include "MainScreen.h"

#include "Global.h"
#include "GUITexture.h"
#include "Data.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "Main.h"
#include "Client.h"
#include "MapScreen.h"
#include "LobbyScreen.h"
#include "MainScreenGlow.h"
#include "PromptBox.h"
#include "Message.h"
#include "GUITextbox.h"

MainScreen::MainScreen()
    : GUIContainer(ANCHOR_NORTHWEST, {0, 0}, {_SCREEN_WIDTH, _SCREEN_HEIGHT}, NULL, NULL)
{
    glowList_ = new LinkedList<MainScreenGlow*>();

    GUITexture* main_title = new GUITexture(ANCHOR_NORTHWEST, {20, 20}, _title_title, {499, 42}, this);
    addObject(main_title);
    GUITexture* main_subtitle = new GUITexture(ANCHOR_NORTHWEST, {20, 82}, _title_subtitle, {390, 10}, this);
    addObject(main_subtitle);

    // options container
    optionsContainer_ = new GUIContainer(ANCHOR_SOUTHWEST, {0, -500}, {1000, 500}, this, NULL);
    GUIButton* options_backbutton = new GUIButton(ANCHOR_SOUTHWEST, {20, -70}, "BACK", optionsContainer_, [](){_mainScreen->toggleOptions();});
    optionsContainer_->addObject(options_backbutton);
    GUISlider* options_slider_sound = new GUISlider(ANCHOR_SOUTHWEST, {270, -140}, {200, 50}, optionsContainer_,
                                          []( float d ){ Mix_Volume(-1, (int)(d*128)); });
    optionsContainer_->addObject(options_slider_sound);
    GUITexture* options_label_soundslider = new GUITexture(ANCHOR_SOUTHWEST, {20, -150}, "Sound Volume:", 50, optionsContainer_);
    optionsContainer_->addObject(options_label_soundslider);
    GUISlider* options_slider_music = new GUISlider(ANCHOR_SOUTHWEST, {270, -210}, {200, 50}, optionsContainer_,
                                          []( float d ){ Mix_VolumeMusic((int)(d*128)); });
    optionsContainer_->addObject(options_slider_music);
    GUITexture* options_label_musicslider = new GUITexture(ANCHOR_SOUTHWEST, {20, -220}, "Music Volume:", 50, optionsContainer_);
    optionsContainer_->addObject(options_label_musicslider);
    GUIButton* options_fullscreenbutton = new GUIButton(ANCHOR_SOUTHWEST, {20, -280}, "Toggle Fullscreen", optionsContainer_,
                                                        [](){toggleFullscreen();});
    optionsContainer_->addObject(options_fullscreenbutton);
    addObject(optionsContainer_);

    optionsContainer_->setMovable(false);
	optionsContainer_->setTransparency(0);

    // main container
    mainContainer_ = new GUIContainer(ANCHOR_SOUTHWEST, {0, -500}, {1000, 500}, this, NULL);
    int ln = 1;

    GUIButton* button_quit = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {73, 41}, mainContainer_,
                                           []()
    {
        Mix_PlayChannel(-1, _sound_move_player, 0);
        _quit = true;
    },
    _main_button_quit,
    _main_button_quit_over);
    mainContainer_->addObject(button_quit);

    GUIButton* button_options = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {138, 41}, mainContainer_,
		[]() {
			_mainScreen->toggleOptions();
			Mix_PlayChannel(-1, _sound_move_player, 0);
		},
    _main_button_options,
    _main_button_options_over);
    mainContainer_->addObject(button_options);

    GUIButton* button_achievements = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {242, 41}, mainContainer_,
		[]() {
			printf("placeholder: goto achievs");
			Mix_PlayChannel(-1, _sound_move_player, 0);
		},
		_main_button_achievements,
		_main_button_achievements_over);
    mainContainer_->addObject(button_achievements);

	GUIButton* button_multiplayer = new GUIButton(ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ }, {216, 41}, mainContainer_,
		[]() {
			_mainScreen->showIPEntry();
			_mainScreen->hideMainContainer();
		},
		_main_button_multiplayer,
		_main_button_multiplayer_over);
	mainContainer_->addObject(button_multiplayer);

    GUIButton* button_freeform = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {320, 41}, mainContainer_,
        []() {
			printf("placeholder: goto freeform map");
			Mix_PlayChannel(-1, _sound_move_player, 0);
		},
		_main_button_freeform,
		_main_button_freeform_over);
    mainContainer_->addObject(button_freeform);

    GUIButton* button_nightfall = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {349, 41}, mainContainer_,
        []() {
			printf("placeholder: goto nightfall campaign map");
			_mapScreen->switchMap(MAPPRESET_NIGHTFALL);
			_currScreen = _mapScreen;
			Mix_PlayMusic(_music_map_ambient, -1);
			Mix_PlayChannel(-1, _sound_move_player, 0);
		},
		_main_button_nightfall,
		_main_button_nightfall_over);
    mainContainer_->addObject(button_nightfall);

    GUIButton* button_classic = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {315, 41}, mainContainer_,
        []() {
			_mapScreen->switchMap(MAPPRESET_CLASSIC);
			_currScreen = _mapScreen;
			Mix_PlayMusic(_music_map_ambient, -1);
			Mix_PlayChannel(-1, _sound_move_player, 0);
		},
		_main_button_classic,
		_main_button_classic_over);
    mainContainer_->addObject(button_classic);

    addObject(mainContainer_);
    mainContainer_->setMovable(false);

	// IP prompt
	IPEntryContainer_ = new GUIContainer(ANCHOR_CENTER, { -5000, -5000 }, { 10000, 10000 },  this, NULL);
	IPEntryBox_ = new PromptBox(ANCHOR_CENTER, { -300, -100 }, { 600, 200 }, this, "Enter IP of server to connect to:", 
		[]() {_mainScreen->submitIPEntry(); _mainScreen->hideMainContainer(); },
		[]() {_mainScreen->hideIPEntry(); _mainScreen->showMainContainer(); });
	IPEntryContainer_->addObject(IPEntryBox_);
	IPEntryContainer_->setTransparency(0);
	addObject(IPEntryContainer_);

	// login prompt
	loginEntryContainer_ = new GUIContainer(ANCHOR_CENTER, { -400, -200 }, { 800, 400 }, this, _color_bkg_standard);
	GUITexture* usernamePrompt = new GUITexture(ANCHOR_NORTHWEST, { 20, 20 }, "Username:", 50, loginEntryContainer_);
	loginEntryContainer_->addObject(usernamePrompt);
	loginUsername_ = new GUITextbox(ANCHOR_NORTHWEST, { 20, 80 }, { 760, 40 }, loginEntryContainer_, DEFAULT_MSG_TEXTSIZE);
	loginEntryContainer_->addObject(loginUsername_);
	GUITexture* passwordPrompt = new GUITexture(ANCHOR_NORTHWEST, { 20, 140 }, "Password:", 50, loginEntryContainer_);
	loginEntryContainer_->addObject(passwordPrompt);
	loginPassword_ = new GUITextbox(ANCHOR_NORTHWEST, { 20, 200 }, { 760, 40 }, loginEntryContainer_, DEFAULT_MSG_TEXTSIZE);
	loginEntryContainer_->addObject(loginPassword_);
	loginCancelButton_ = new GUIButton(ANCHOR_SOUTHWEST, { 20, -80 }, "Cancel", loginEntryContainer_, []() {_mainScreen->loginHide(); _client->disconnect(); });
	loginEntryContainer_->addObject(loginCancelButton_);
	loginCreateButton_ = new GUIButton(ANCHOR_SOUTH, { -40, -80 }, "Create Account", loginEntryContainer_, []() {_mainScreen->loginCreate(); });
	loginEntryContainer_->addObject(loginCreateButton_);
	loginSubmitButton_ = new GUIButton(ANCHOR_SOUTHEAST, { -120, -80 }, "Login", loginEntryContainer_, []() {_mainScreen->loginSubmit(); });
	loginEntryContainer_->addObject(loginSubmitButton_);
	loginEntryContainer_->setTransparency(0);
	addObject(loginEntryContainer_);

	// misc
    textBkgDisplacement_ = 0;
}

MainScreen::~MainScreen()
{
    //dtor
}

void MainScreen::draw()
{
    // clear the screen (black)
    SDL_SetRenderDrawColor(_renderer, 0, 0, 40, 0);
    SDL_RenderClear(_renderer);

    // draw all particulates
    glowList_->forEach([](MainScreenGlow* g){ g->draw(); });

    // draw the text chunk
    SDL_Rect destRect;
    SDL_QueryTexture(_main_bkgdata, NULL, NULL, &destRect.w, &destRect.h);
    for (int x = 0; x < _SCREEN_WIDTH; x += destRect.w)
    {
        for (int y = -textBkgDisplacement_; y < _SCREEN_HEIGHT + textBkgDisplacement_; y += destRect.h)
        {
            destRect.x = x;
            destRect.y = y;
            SDL_RenderCopy(_renderer, _main_bkgdata, NULL, &destRect);
        }
    }

    // draw GUIObjects
    drawContents();
}

void MainScreen::tick(int ms)
{
    // tick all GUIObjects
    GUIContainer::tick(ms);

    // change the text vertical displacement
    textBkgDisplacement_++;
    int txtHeight;
    SDL_QueryTexture(_main_bkgdata, NULL, NULL, NULL, &txtHeight);
    if (textBkgDisplacement_ > txtHeight) textBkgDisplacement_ -= txtHeight;

    // tick all particulates
    Iterator<MainScreenGlow*> it = glowList_->getIterator();
    while (it.hasNext())
    {
        it.next()->tick(ms);
    }

    // check for dead/OOB particulates
    LinkedList<MainScreenGlow*> delList = LinkedList<MainScreenGlow*>();
    Iterator<MainScreenGlow*> it3 = glowList_->getIterator();
    while (it3.hasNext())
    {
        MainScreenGlow* currObj = it3.next();
        if (currObj->getTransparent())
        {
            delList.addFirst(currObj);
        }
        else if (currObj->getPos().x + 200 < 0 ||
                 currObj->getPos().y + 200 < 0 ||
                 currObj->getPos().x > _SCREEN_WIDTH ||
                 currObj->getPos().y > _SCREEN_HEIGHT)
        {
            delList.addFirst(currObj);
        }
    }

    Iterator<MainScreenGlow*> it2 = delList.getIterator();
    while (it2.hasNext())
    {
        MainScreenGlow* currObj = it2.next();
        glowList_->remove(currObj);
        delete currObj;
    }

    // the longer the delay, the more likely to add a particle
    // at 50ms, a particle is guaranteed to be added
    if (rand() % 50 < ms)
    {
        MainScreenGlow* newGlow = new MainScreenGlow({rand()%_SCREEN_WIDTH, rand()%_SCREEN_HEIGHT});
        glowList_->addLast(newGlow);
    }
}

void MainScreen::toggleOptions()
{
    if (optionsContainer_->isVisible())
    {
        optionsContainer_->fade(0, 1000);
        mainContainer_->fade(255, 1000);
    }
    else
    {
        optionsContainer_->fade(255, 1000);
        mainContainer_->fade(0, 1000);
    }
}

void MainScreen::showIPEntry()
{
	IPEntryContainer_->setTransparency(255);
	IPEntryBox_->clearContents();
	_activeTextbox = IPEntryBox_->getTextbox();
}

void MainScreen::hideIPEntry()
{
	IPEntryContainer_->setTransparency(0);
}

void MainScreen::submitIPEntry()
{
	_client->connectIP(IPEntryBox_->getContents());
}

void MainScreen::loginShow()
{
	loginEntryContainer_->setTransparency(255);
	loginUsername_->clearContents();
	loginPassword_->clearContents();
	_activeTextbox = loginUsername_;
}

void MainScreen::loginHide()
{
	loginEntryContainer_->setTransparency(0);
}

void MainScreen::loginCreate()
{
	Message m;
	m.type = MSGTYPE_CREATEUSER;
	m.infoType = MSGINFOTYPE_USERNAME;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, loginUsername_->getContents().c_str(), DEFAULT_MSG_TEXTSIZE);
	_client->sendMessage(m);

	m.infoType = MSGINFOTYPE_PASSWORD;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, loginPassword_->getContents().c_str(), DEFAULT_MSG_TEXTSIZE);
	_client->sendMessage(m);
}

void MainScreen::loginSubmit()
{
	Message m;
	m.type = MSGTYPE_LOGIN;
	m.infoType = MSGINFOTYPE_USERNAME;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, loginUsername_->getContents().c_str(), DEFAULT_MSG_TEXTSIZE);
	_client->sendMessage(m);

	m.infoType = MSGINFOTYPE_PASSWORD;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, loginPassword_->getContents().c_str(), DEFAULT_MSG_TEXTSIZE);
	_client->sendMessage(m);
}

void MainScreen::loginClear()
{
	loginUsername_->clearContents();
	loginPassword_->clearContents();
	_activeTextbox = loginUsername_;
}

void MainScreen::hideMainContainer()
{
	mainContainer_->setTransparency(0);
}

void MainScreen::showMainContainer()
{
	mainContainer_->setTransparency(255);
}

void MainScreen::keyPress(char c)
{
	if (IPEntryContainer_->isVisible())
		IPEntryBox_->addChar(c);
	if (loginEntryContainer_->isVisible())
	{
		if (c == 13) // return
		{
			loginSubmitButton_->mouseDown();
			loginSubmitButton_->mouseUp();
		}
		else if (c == 27) // escape
		{
			loginCancelButton_->mouseDown();
			loginCancelButton_->mouseUp();
		}
		else if (c == 9) // tab
		{
			if (_activeTextbox == loginUsername_)
				_activeTextbox = loginPassword_;
			else if (_activeTextbox == loginPassword_)
				_activeTextbox = loginUsername_;
		}
		else if (loginUsername_ == _activeTextbox)
			loginUsername_->addChar(c);
		else if (loginPassword_ == _activeTextbox)
			loginPassword_->addChar(c);
	}
}