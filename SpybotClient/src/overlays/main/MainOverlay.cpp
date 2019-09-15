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
#include "BackgroundOverlay.h"
#include "User.h"
#include "Server.h"
#include "UserDisplay.h"
#include "AchievementButton.h"
#include "GUIEffectFade.h"
#include "GUIEffectTranslate.h"

MainOverlay::MainOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_clear) {
	GUITexture* main_title = new GUITexture(this, ANCHOR_NORTHWEST, { 20, 20 }, { 499, 42 }, _title_title);
	addObject(main_title);
	GUITexture* main_subtitle = new GUITexture(this, ANCHOR_NORTHWEST, { 20, 82 }, { 390, 10 }, _title_subtitle, this);
	addObject(main_subtitle);

	/*
	OPTIONS CONTAINER
	*/

	optionsContainer_ = new GUIContainer(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 1000, 500 }, _color_clear);
	GUIButton* options_backbutton = new GUIButton(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -20 }, "BACK", 50,
		[] () {
		_mainOverlay->hideOptions(1000);
		_mainOverlay->showMainContainer(1000);
	});
	optionsContainer_->addObject(options_backbutton);
	GUISlider* options_slider_sound = new GUISlider(optionsContainer_, ANCHOR_SOUTHWEST, { 270, -150 }, { 200, 50 },
		[] (float d) {
		Mix_Volume(-1, (int)(d * 128));
	});
	optionsContainer_->addObject(options_slider_sound);
	GUITexture* options_label_soundslider = new GUITexture(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -150 }, "SOUND VOLUME:", 50);
	optionsContainer_->addObject(options_label_soundslider);
	GUISlider* options_slider_music = new GUISlider(optionsContainer_, ANCHOR_SOUTHWEST, { 270, -210 }, { 200, 50 },
		[] (float d) {
		Mix_VolumeMusic((int)(d * 128));
	});
	optionsContainer_->addObject(options_slider_music);
	GUITexture* options_label_musicslider = new GUITexture(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -220 }, "MUSIC VOLUME:", 50);
	optionsContainer_->addObject(options_label_musicslider);
	GUIButton* options_fullscreenbutton = new GUIButton(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -280 }, "TOGGLE FULLSCREEN", 50,
		[] () {
		toggleFullscreen();
	});
	optionsContainer_->addObject(options_fullscreenbutton);
	addObject(optionsContainer_);

	optionsContainer_->setMovable(false);
	optionsContainer_->setTransparency(0);

	/*
	CAMPAIGN SELECTION CONTAINER
	*/

	campaignContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 700, 470 }, _color_clear);
	this->addObject(campaignContainer_);
	campaignContainer_->setTransparency(0);

	GUIContainer* campaignSelectContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 700, 370 }, _color_bkg_standard);
	campaignContainer_->addObject(campaignSelectContainer_);

	GUIContainer* classicContainer = new GUIContainer(campaignSelectContainer_, ANCHOR_NORTHWEST, { 10, 10 }, { 220, 300 }, _color_bkg_standard);
	campaignSelectContainer_->addObject(classicContainer);
	GUITexture* classicCampaignBkg = new GUITexture(classicContainer, ANCHOR_NORTHWEST, { 10, 10 }, { 200, 200 }, _main_campaign_bkg);
	classicContainer->addObject(classicCampaignBkg);
	GUITexture* classicCampaignIcon = new GUITexture(classicContainer, ANCHOR_NORTHWEST, { 10, 10 }, { 180, 180 }, _main_campaign_classic);
	classicContainer->addObject(classicCampaignIcon);
	GUIButton* classicCampaignButton = new GUIButton(classicContainer, ANCHOR_SOUTH, { 0, -10 }, "CLASSIC", 50,
		[] () {
		_connectionManager->connectToLocalServer("levels/classic");
		_mapOverlay->switchMap(MAPPRESET_CLASSIC);
		_mainOverlay->hideCampaigns(1000);
		_mainOverlay->showLocalLoginContainer(1000);
		_mainOverlay->refreshUsers();
		Mix_PlayMusic(_music_map_ambient, -1);
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	classicContainer->addObject(classicCampaignButton);

	GUIContainer* nightfallContainer = new GUIContainer(campaignSelectContainer_, ANCHOR_NORTH, { 0, 10 }, { 220, 300 }, _color_bkg_standard);
	campaignSelectContainer_->addObject(nightfallContainer);
	GUITexture* nightfallCampaignBkg = new GUITexture(nightfallContainer, ANCHOR_NORTHWEST, { 10, 10 }, { 200, 200 }, _main_campaign_bkg);
	nightfallContainer->addObject(nightfallCampaignBkg);
	GUITexture* nightfallCampaignIcon = new GUITexture(nightfallContainer, ANCHOR_NORTHWEST, { 10, 10 }, { 180, 180 }, _main_campaign_nightfall);
	nightfallContainer->addObject(nightfallCampaignIcon);
	GUIButton* nightfallCampaignButton = new GUIButton(nightfallContainer, ANCHOR_SOUTH, { 0, -10 }, "NIGHTFALL", 50,
		[] () {
		_connectionManager->connectToLocalServer("levels/nightfall");
		_mapOverlay->switchMap(MAPPRESET_NIGHTFALL);
		_mainOverlay->hideCampaigns(1000);
		_mainOverlay->showLocalLoginContainer(1000);
		_mainOverlay->refreshUsers();
		Mix_PlayMusic(_music_map_ambient, -1);
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	nightfallContainer->addObject(nightfallCampaignButton);

	GUIContainer* proceduralContainer = new GUIContainer(campaignSelectContainer_, ANCHOR_NORTHEAST, { -10, 10 }, { 220, 300 }, _color_bkg_standard);
	campaignSelectContainer_->addObject(proceduralContainer);
	GUITexture* proceduralCampaignBkg = new GUITexture(proceduralContainer, ANCHOR_NORTHWEST, { 10, 10 }, { 200, 200 }, _main_campaign_bkg);
	proceduralContainer->addObject(proceduralCampaignBkg);
	GUITexture* proceduralCampaignIcon = new GUITexture(proceduralContainer, ANCHOR_NORTHWEST, { 10, 10 }, { 180, 180 }, _main_campaign_procedural);
	proceduralContainer->addObject(proceduralCampaignIcon);
	GUIButton* proceduralCampaignButton = new GUIButton(proceduralContainer, ANCHOR_SOUTH, { 0, -10 }, "PROCEDURAL", 50,
		[] () {
		printf("placeholder: goto freeform map");
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	proceduralContainer->addObject(proceduralCampaignButton);

	GUIButton* campaignBackButton = new GUIButton(campaignSelectContainer_, ANCHOR_SOUTHWEST, { 10, 0 }, "BACK", 50,
		[] () {
		_mainOverlay->hideCampaigns(1000);
		_mainOverlay->showMainContainer(1000);
	});
	campaignSelectContainer_->addObject(campaignBackButton);

	GUITexture* campaignSelectTitle_ = new GUITexture(this, ANCHOR_CENTER, { 0, -225 }, "SELECT A CAMPAIGN", 80);
	campaignContainer_->addObject(campaignSelectTitle_);

	/*
	MAIN CONTAINER
	*/

	mainContainer_ = new GUIContainer(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 1000, 500 }, _color_clear);
	mainContainer_->setMovable(false);
	this->addObject(mainContainer_);
	int ln = 0;
	GUIButton* button_quit = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "QUIT", 50,
		[] () {
		Mix_PlayChannel(-1, _sound_move_player, 0);
		_quit = true;
	});
	mainContainer_->addObject(button_quit);
	GUIButton* button_options = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "OPTIONS", 50,
		[] () {
		_mainOverlay->showOptions(1000);
		_mainOverlay->hideMainContainer(1000);
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_options);
	GUIButton* button_achievements = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "ACHIEVEMENTS", 50, [] () {
		_mainOverlay->showAchievementsContainer(1000);
		_mainOverlay->hideMainContainer(1000);
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_achievements);
	GUIButton* button_multiplayer = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "MULTIPLAYER", 50,
		[] () {
		_mainOverlay->showIPEntry(1000);
		_mainOverlay->hideMainContainer(1000);
	});
	mainContainer_->addObject(button_multiplayer);
	GUIButton* button_campaign = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "CAMPAIGN", 50,
		[] () {
		_mainOverlay->showCampaigns(1000);
		_mainOverlay->hideMainContainer(1000);
	});
	mainContainer_->addObject(button_campaign);

	/*
	IP PROMPT CONTAINER
	*/

	IPEntryBox_ = new PromptBox(ANCHOR_CENTER, { 0, 0 }, { 600, 200 }, this, "SERVER IP:",
		[] () {
		_mainOverlay->submitIPEntry();
	}, [] () {
		_mainOverlay->hideIPEntry(1000);
		_mainOverlay->showMainContainer(1000);
	});
	this->addObject(IPEntryBox_);
	IPEntryBox_->setTransparency(0);

	/*
	LOGIN ENTRY CONTAINER
	*/

	loginEntryContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 800, 400 }, _color_bkg_standard);
	this->addObject(loginEntryContainer_);
	loginEntryContainer_->setTransparency(0);
	GUITexture* usernamePrompt = new GUITexture(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 20 }, "USERNAME:", 50);
	loginEntryContainer_->addObject(usernamePrompt);
	loginUsername_ = new GUITextbox(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 80 }, { 760, 40 }, DEFAULT_MSG_TEXTSIZE / 2 - 5, false);
	loginEntryContainer_->addObject(loginUsername_);
	GUITexture* passwordPrompt = new GUITexture(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 140 }, "PASSWORD:", 50);
	loginEntryContainer_->addObject(passwordPrompt);
	loginPassword_ = new GUITextbox(loginEntryContainer_, ANCHOR_NORTHWEST, { 20, 200 }, { 760, 40 }, DEFAULT_MSG_TEXTSIZE / 2 - 5, true);
	loginEntryContainer_->addObject(loginPassword_);
	loginCancelButton_ = new GUIButton(loginEntryContainer_, ANCHOR_SOUTHWEST, { 20, -20 }, "CANCEL", 50,
		[] () {
		_mainOverlay->loginHide(1000);
		_connectionManager->disconnect();
	});
	loginEntryContainer_->addObject(loginCancelButton_);
	loginCreateButton_ = new GUIButton(loginEntryContainer_, ANCHOR_SOUTH, { 0, -20 }, "CREATE ACCOUNT", 50,
		[] () {
		_mainOverlay->loginCreate();
	});
	loginEntryContainer_->addObject(loginCreateButton_);
	loginSubmitButton_ = new GUIButton(loginEntryContainer_, ANCHOR_SOUTHEAST, { -20, -20 }, "LOGIN", 50,
		[] () {
		_mainOverlay->loginSubmit();
	});
	loginEntryContainer_->addObject(loginSubmitButton_);

	/*
	LOCAL LOGIN CONTAINER
	*/

	localLoginContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 640, 450 }, _color_clear);
	this->addObject(localLoginContainer_);
	localLoginContainer_->setTransparency(0);

	GUITexture* saveContainerTitle_ = new GUITexture(localLoginContainer_, ANCHOR_CENTER, { 0, -210 }, "SELECT A SAVEFILE", 80);
	localLoginContainer_->addObject(saveContainerTitle_);

	saveContainer_ = new GUIContainer(localLoginContainer_, ANCHOR_CENTER, { 0, 0 }, { 640, 350 }, _color_bkg_standard);
	localLoginContainer_->addObject(saveContainer_);

	localLoginBackButton_ = new GUIButton(saveContainer_, ANCHOR_SOUTHWEST, { 10, 0 }, "BACK", 50, 
		[] () {
		_mainOverlay->hideLocalLoginContainer(1000);
		_mainOverlay->showCampaigns(1000);
	});
	saveContainer_->addObject(localLoginBackButton_);

	user1Container_ = NULL;
	user2Container_ = NULL;
	user3Container_ = NULL;

	/*
	ACHIEVEMENTS CONTAINER
	*/

	achievementsContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0, 0 }, { 800, 600 }, _color_clear);
	this->addObject(achievementsContainer_);
	achievementsContainer_->setTransparency(0);

	GUIContainer* achievementCollectionContainer_ = new GUIContainer(achievementsContainer_, ANCHOR_NORTH, { 0, 0 }, { 800, 520 }, _color_bkg_standard);
	achievementsContainer_->addObject(achievementCollectionContainer_);
	int xPos = 10;
	int yPos = 10;
	for (int i = 0; i < ACHIEVEMENT_NUM_ACHIEVEMENTS; i++) {
		AchievementButton* achievementButton = new AchievementButton(achievementCollectionContainer_, ANCHOR_NORTHWEST, { xPos, yPos }, (ACHIEVEMENT)i);
		achievementCollectionContainer_->addObject(achievementButton);
		xPos += 60;
		if (xPos > 740) {
			xPos = 10;
			yPos += 60;
		}
	}

	achievementsDisplayContainer_ = new GUIContainer(achievementsContainer_, ANCHOR_SOUTH, { 0, 0 }, { 800, 70 }, _color_bkg_standard);
	achievementsContainer_->addObject(achievementsDisplayContainer_);
	achievementTitle_ = NULL;
	achievementDescription_ = NULL;
	achievementIcon_ = NULL;

	GUIButton* achievementsBackButton_ = new GUIButton(achievementsContainer_, ANCHOR_SOUTHWEST, { 10, 0 }, "BACK", 50, 
		[] () {
		_mainOverlay->hideAchievementsContainer(1000);
		_mainOverlay->showMainContainer(1000);
	});
	achievementsContainer_->addObject(achievementsBackButton_);
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

void MainOverlay::showIPEntry(int fadeDuration) {
	IPEntryBox_->addEffect(new GUIEffectFade(0, fadeDuration, IPEntryBox_->getTransparency(), 255));
	IPEntryBox_->clearContents();
	_activeTextbox = IPEntryBox_->getTextbox();
}

void MainOverlay::hideIPEntry(int fadeDuration) {
	IPEntryBox_->addEffect(new GUIEffectFade(0, fadeDuration, IPEntryBox_->getTransparency(), 0));
}

void MainOverlay::submitIPEntry() {
	_connectionManager->connectToExternalServer(IPEntryBox_->getContents());
}

void MainOverlay::loginShow(int fadeDuration) {
	loginEntryContainer_->addEffect(new GUIEffectFade(0, fadeDuration, loginEntryContainer_->getTransparency(), 255));
	loginUsername_->clearContents();
	loginPassword_->clearContents();
	_activeTextbox = loginUsername_;
}

void MainOverlay::loginHide(int fadeDuration) {
	loginEntryContainer_->addEffect(new GUIEffectFade(0, fadeDuration, loginEntryContainer_->getTransparency(), 0));
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

void MainOverlay::showMainContainer(int fadeDuration) {
	mainContainer_->addEffect(new GUIEffectTranslate(0, fadeDuration, Coord{ 100, 0 }, Coord{ 0, 0 }));
	mainContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 255));
}

void MainOverlay::hideMainContainer(int fadeDuration) {
	mainContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 0));
}

void MainOverlay::showOptions(int fadeDuration) {
	optionsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, optionsContainer_->getTransparency(), 255));
}

void MainOverlay::hideOptions(int fadeDuration) {
	optionsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, optionsContainer_->getTransparency(), 0));
}

void MainOverlay::showCampaigns(int fadeDuration) {
	campaignContainer_->addEffect(new GUIEffectFade(0, fadeDuration, campaignContainer_->getTransparency(), 255));
}

void MainOverlay::hideCampaigns(int fadeDuration) {
	campaignContainer_->addEffect(new GUIEffectFade(0, fadeDuration, campaignContainer_->getTransparency(), 0));
}

void MainOverlay::showLocalLoginContainer(int fadeDuration) {
	localLoginContainer_->addEffect(new GUIEffectFade(0, fadeDuration, localLoginContainer_->getTransparency(), 255));
}

void MainOverlay::hideLocalLoginContainer(int fadeDuration) {
	localLoginContainer_->addEffect(new GUIEffectFade(0, fadeDuration, localLoginContainer_->getTransparency(), 0));
}

void MainOverlay::showAchievementsContainer(int fadeDuration) {
	achievementsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, achievementsContainer_->getTransparency(), 255));
}

void MainOverlay::hideAchievementsContainer(int fadeDuration) {
	achievementsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, achievementsContainer_->getTransparency(), 0));
}

void MainOverlay::hideAll(int fadeDuration) {
	mainContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 0));
	optionsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, optionsContainer_->getTransparency(), 0));
	campaignContainer_->addEffect(new GUIEffectFade(0, fadeDuration, campaignContainer_->getTransparency(), 0));
	loginEntryContainer_->addEffect(new GUIEffectFade(0, fadeDuration, loginEntryContainer_->getTransparency(), 0));
	localLoginContainer_->addEffect(new GUIEffectFade(0, fadeDuration, localLoginContainer_->getTransparency(), 0));
	achievementsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, achievementsContainer_->getTransparency(), 0));
}

void MainOverlay::keyPress(char c) {
	if (IPEntryBox_->isVisible())
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

void MainOverlay::refreshUsers() {
	// remove all previous contents
	saveContainer_->getContents()->removeAll();
	if (user1Container_ != NULL) {
		delete user1Container_;
		user1Container_ = NULL;
	}
	if (user2Container_ != NULL) {
		delete user2Container_;
		user2Container_ = NULL;
	}
	if (user3Container_ != NULL) {
		delete user3Container_;
		user3Container_ = NULL;
	}

	// populate container
	if (_server != NULL) {
		LinkedList<User*>* users = _server->getUsers();
		int xDisp = 10;
		user1Container_ = new UserDisplay(ANCHOR_NORTHWEST, { xDisp, 10 }, { 200, 280 }, saveContainer_, users->getObjectAt(0));
		saveContainer_->addObject(user1Container_);
		xDisp += 200 + 10;
		user2Container_ = new UserDisplay(ANCHOR_NORTHWEST, { xDisp, 10 }, { 200, 280 }, saveContainer_, users->getObjectAt(1));
		saveContainer_->addObject(user2Container_);
		xDisp += 200 + 10;
		user3Container_ = new UserDisplay(ANCHOR_NORTHWEST, { xDisp, 10 }, { 200, 280 }, saveContainer_, users->getObjectAt(2));
		saveContainer_->addObject(user3Container_);
	}

	// re-add back button
	localLoginContainer_->addObject(localLoginBackButton_);
}

void MainOverlay::setSelectedAchievement(ACHIEVEMENT a) {
	achievementsDisplayContainer_->getContents()->removeAll();
	if (achievementTitle_ != NULL) {
		delete achievementTitle_;
		achievementTitle_ = NULL;
	}
	if (achievementDescription_ != NULL) {
		delete achievementDescription_;
		achievementDescription_ = NULL;
	}
	if (achievementIcon_ != NULL) {
		delete achievementIcon_;
		achievementIcon_ = NULL;
	}

	achievementTitle_ = new GUITexture(achievementsDisplayContainer_, ANCHOR_NORTHEAST, { -70, 0 }, _achievement_name[a], 30);
	achievementTitle_->setTransparency(0);
	achievementTitle_->addEffect(new GUIEffectFade(250, 500, 0, 255));
	achievementTitle_->addEffect(new GUIEffectTranslate(250, 500, { -100, 0 }, { -70, 0 }));
	achievementsDisplayContainer_->addObject(achievementTitle_);
	achievementDescription_ = new GUITexture(achievementsDisplayContainer_, ANCHOR_SOUTHEAST, { -70, 0 }, _achievement_description[a], 30);
	achievementDescription_->setTransparency(0);
	achievementDescription_->addEffect(new GUIEffectFade(500, 500, 0, 255));
	achievementDescription_->addEffect(new GUIEffectTranslate(500, 500, { -100, 0 }, { -70, 0 }));
	achievementsDisplayContainer_->addObject(achievementDescription_);
	if (_progressAchievements[a]) {
		achievementIcon_ = new GUITexture(achievementsDisplayContainer_, ANCHOR_EAST, { -10, 0 }, { 50, 50 }, _achievement_texture[a]);
	} else {
		achievementIcon_ = new GUITexture(achievementsDisplayContainer_, ANCHOR_EAST, { -10, 0 }, { 50, 50 }, _achievement_locked);
	}
	achievementIcon_->setTransparency(0);
	achievementIcon_->addEffect(new GUIEffectFade(0, 500, 0, 255));
	achievementsDisplayContainer_->addObject(achievementIcon_);
}