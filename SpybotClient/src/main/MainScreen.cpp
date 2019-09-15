#include "Standard.h"
#include "MainScreen.h"

#include "Global.h"
#include "GUITexture.h"
#include "DataContainer.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "Main.h"
#include "Client.h"
#include "MapScreen.h"
#include "LobbyScreen.h"
#include "MainScreenGlow.h"
#include "TextEntryBox.h"

MainScreen::MainScreen()
    : GUIContainer(ANCHOR_NORTHWEST, {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT}, NULL, NULL)
{
    glowList = new LinkedList<MainScreenGlow*>();

    GUITexture* main_title = new GUITexture(ANCHOR_NORTHWEST, {20, 20}, dataContainer->title_title, {499, 42}, this);
    addObject(main_title);
    GUITexture* main_subtitle = new GUITexture(ANCHOR_NORTHWEST, {20, 82}, dataContainer->title_subtitle, {390, 10}, this);
    addObject(main_subtitle);

    // options container
    optionsContainer = new GUIContainer(ANCHOR_SOUTHWEST, {0, -500}, {1000, 500}, this, NULL);
    GUIButton* options_backbutton = new GUIButton(ANCHOR_SOUTHWEST, {20, -70}, "BACK", optionsContainer, [](){mainScreen->toggleOptions();});
    optionsContainer->addObject(options_backbutton);
    GUISlider* options_slider_sound = new GUISlider(ANCHOR_SOUTHWEST, {270, -140}, {200, 50}, optionsContainer,
                                          []( float d ){ Mix_Volume(-1, (int)(d*128)); });
    optionsContainer->addObject(options_slider_sound);
    GUITexture* options_label_soundslider = new GUITexture(ANCHOR_SOUTHWEST, {20, -150}, "Sound Volume:", optionsContainer);
    optionsContainer->addObject(options_label_soundslider);
    GUISlider* options_slider_music = new GUISlider(ANCHOR_SOUTHWEST, {270, -210}, {200, 50}, optionsContainer,
                                          []( float d ){ Mix_VolumeMusic((int)(d*128)); });
    optionsContainer->addObject(options_slider_music);
    GUITexture* options_label_musicslider = new GUITexture(ANCHOR_SOUTHWEST, {20, -220}, "Music Volume:", optionsContainer);
    optionsContainer->addObject(options_label_musicslider);
    GUIButton* options_fullscreenbutton = new GUIButton(ANCHOR_SOUTHWEST, {20, -280}, "Toggle Fullscreen", optionsContainer,
                                                        [](){toggleFullscreen();});
    optionsContainer->addObject(options_fullscreenbutton);
    addObject(optionsContainer);

    optionsContainer->setMovable(false);
	optionsContainer->setTransparency(0);

    // main container
    mainContainer = new GUIContainer(ANCHOR_SOUTHWEST, {0, -500}, {1000, 500}, this, NULL);
    int ln = 1;

    GUIButton* button_quit = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {73, 41}, mainContainer,
                                           []()
    {
        Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
        quit = true;
    },
    dataContainer->main_button_quit,
    dataContainer->main_button_quit_over);
    mainContainer->addObject(button_quit);

    GUIButton* button_options = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {138, 41}, mainContainer,
		[]() {
			mainScreen->toggleOptions();
			Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
		},
    dataContainer->main_button_options,
    dataContainer->main_button_options_over);
    mainContainer->addObject(button_options);

    GUIButton* button_achievements = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {242, 41}, mainContainer,
		[]() {
			printf("placeholder: goto achievs");
			Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
		},
		dataContainer->main_button_achievements,
		dataContainer->main_button_achievements_over);
    mainContainer->addObject(button_achievements);

	GUIButton* button_multiplayer = new GUIButton(ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ }, {216, 41}, mainContainer,
		[]() {
			mainScreen->showIPEntry();
		},
		dataContainer->main_button_multiplayer,
		dataContainer->main_button_multiplayer_over);
	mainContainer->addObject(button_multiplayer);

    GUIButton* button_freeform = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {320, 41}, mainContainer,
        []() {
			printf("placeholder: goto freeform map");
			Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
		},
		dataContainer->main_button_freeform,
		dataContainer->main_button_freeform_over);
    mainContainer->addObject(button_freeform);

    GUIButton* button_nightfall = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {349, 41}, mainContainer,
        []() {
			printf("placeholder: goto nightfall campaign map");
			mapScreen->switchMap(MAPPRESET_NIGHTFALL);
			currScreen = mapScreen;
			Mix_PlayMusic(dataContainer->music_map_ambient, -1);
			Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
		},
		dataContainer->main_button_nightfall,
		dataContainer->main_button_nightfall_over);
    mainContainer->addObject(button_nightfall);

    GUIButton* button_classic = new GUIButton(ANCHOR_SOUTHWEST, {20, -(41 + 20)*ln++}, {315, 41}, mainContainer,
        []() {
			mapScreen->switchMap(MAPPRESET_CLASSIC);
			currScreen = mapScreen;
			Mix_PlayMusic(dataContainer->music_map_ambient, -1);
			Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
		},
		dataContainer->main_button_classic,
		dataContainer->main_button_classic_over);
    mainContainer->addObject(button_classic);

    addObject(mainContainer);
    mainContainer->setMovable(false);

	// name prompt
	nameEntryContainer = new GUIContainer(ANCHOR_CENTER, { -5000, -5000 }, { 10000, 10000 }, this, NULL);
	nameEntryBox = new TextEntryBox(ANCHOR_CENTER, { -300, -100 }, { 600, 200 }, this, "Enter your name:", []() {mainScreen->submitNameEntry(); }, []() {mainScreen->cancelNameEntry(); });
	nameEntryContainer->addObject(nameEntryBox);
	nameEntryContainer->setTransparency(0);
	addObject(nameEntryContainer);

	// IP prompt
	IPEntryContainer = new GUIContainer(ANCHOR_CENTER, { -5000, -5000 }, { 10000, 10000 },  this, NULL);
	IPEntryBox = new TextEntryBox(ANCHOR_CENTER, { -300, -100 }, { 600, 200 }, this, "Enter IP of server to connect to:", []() {mainScreen->submitIPEntry(); }, []() {mainScreen->cancelIPEntry(); });
	IPEntryContainer->addObject(IPEntryBox);
	IPEntryContainer->setTransparency(0);
	addObject(IPEntryContainer);

	// misc
    textBkgDisplacement = 0;
	showNameEntry();
}

MainScreen::~MainScreen()
{
    //dtor
}

void MainScreen::draw()
{
    // clear the screen (black)
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 40, 0);
    SDL_RenderClear(gRenderer);

    // draw all particulates
    glowList->forEach([](MainScreenGlow* g){ g->draw(); });

    // draw the text chunk
    SDL_Rect destRect;
    SDL_QueryTexture(dataContainer->main_bkgdata, NULL, NULL, &destRect.w, &destRect.h);
    for (int x = 0; x < SCREEN_WIDTH; x += destRect.w)
    {
        for (int y = -textBkgDisplacement; y < SCREEN_HEIGHT + textBkgDisplacement; y += destRect.h)
        {
            destRect.x = x;
            destRect.y = y;
            SDL_RenderCopy(gRenderer, dataContainer->main_bkgdata, NULL, &destRect);
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
    textBkgDisplacement++;
    int txtHeight;
    SDL_QueryTexture(dataContainer->main_bkgdata, NULL, NULL, NULL, &txtHeight);
    if (textBkgDisplacement > txtHeight) textBkgDisplacement -= txtHeight;

    // tick all particulates
    Iterator<MainScreenGlow*> it = glowList->getIterator();
    while (it.hasNext())
    {
        it.next()->tick(ms);
    }

    // check for dead/OOB particulates
    LinkedList<MainScreenGlow*> delList = LinkedList<MainScreenGlow*>();
    Iterator<MainScreenGlow*> it3 = glowList->getIterator();
    while (it3.hasNext())
    {
        MainScreenGlow* currObj = it3.next();
        if (currObj->getTransparent())
        {
            delList.addFirst(currObj);
        }
        else if (currObj->getPos().x + 200 < 0 ||
                 currObj->getPos().y + 200 < 0 ||
                 currObj->getPos().x > SCREEN_WIDTH ||
                 currObj->getPos().y > SCREEN_HEIGHT)
        {
            delList.addFirst(currObj);
        }
    }

    Iterator<MainScreenGlow*> it2 = delList.getIterator();
    while (it2.hasNext())
    {
        MainScreenGlow* currObj = it2.next();
        glowList->remove(currObj);
        delete currObj;
    }

    // the longer the delay, the more likely to add a particle
    // at 50ms, a particle is guaranteed to be added
    if (rand() % 50 < ms)
    {
        MainScreenGlow* newGlow = new MainScreenGlow({rand()%SCREEN_WIDTH, rand()%SCREEN_HEIGHT});
        glowList->addLast(newGlow);
    }
}

void MainScreen::toggleOptions()
{
    if (optionsContainer->isVisible())
    {
        optionsContainer->fade(0, 1000);
        mainContainer->fade(255, 1000);
    }
    else
    {
        optionsContainer->fade(255, 1000);
        mainContainer->fade(0, 1000);
    }
}

void MainScreen::showNameEntry()
{
	nameEntryContainer->setTransparency(255);
	mainContainer->setTransparency(0);
	nameEntryBox->clearContents();
}

void MainScreen::cancelNameEntry()
{
	nameEntryContainer->setTransparency(0);

	mainContainer->setTransparency(255);
}

void MainScreen::submitNameEntry()
{
	username = (nameEntryBox->getContents() == "") ? "null" : nameEntryBox->getContents();
	nameEntryContainer->setTransparency(0);

	mainContainer->setTransparency(255);
}

void MainScreen::showIPEntry()
{
	IPEntryContainer->setTransparency(255);
	mainContainer->setTransparency(0);
	IPEntryBox->clearContents();
}

void MainScreen::cancelIPEntry()
{
	IPEntryContainer->setTransparency(0);

	mainContainer->setTransparency(255);
}

void MainScreen::submitIPEntry()
{
	client->connectIP(IPEntryBox->getContents());
	currScreen = lobbyScreen;

	IPEntryContainer->setTransparency(0);

	mainContainer->setTransparency(255);
}

void MainScreen::keyPress(char c)
{
	if (nameEntryContainer->isVisible())
		nameEntryBox->addChar(c);
	if (IPEntryContainer->isVisible())
		IPEntryBox->addChar(c);
}