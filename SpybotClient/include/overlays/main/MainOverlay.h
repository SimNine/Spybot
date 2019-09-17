#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class GlowSpeck;
class PromptBox;
class GUIButton;
class GUITextbox;
class GUITexture;
class UserDisplay;

class MainOverlay : public GUIContainer {
public:
	MainOverlay();
	virtual ~MainOverlay();

	void draw();
	void tick(int);

	void showCampaigns(int fadeDuration);
	void hideCampaigns(int fadeDuration);

	void showOptions(int fadeDuration);
	void hideOptions(int fadeDuration);

	void showIPEntry(int fadeDuration);
	void hideIPEntry(int fadeDuration);

	void showLoginContainer(int fadeDuration);
	void hideLoginContainer(int fadeDuration);
	void loginCreate();
	void loginSubmit();
	void loginClear();

	void hideMainContainer(int fadeDuration);
	void showMainContainer(int fadeDuration);

	void hideLocalLoginContainer(int fadeDuration);
	void showLocalLoginContainer(int fadeDuration);

	void hideLocalUsernameEntry(int fadeDuration);
	void showLocalUsernameEntry(int fadeDuration);

	void hideCreditsContainer(int fadeDuration);
	void showCreditsContainer(int fadeDuration);

	void showAchievementsContainer(int fadeDuration);
	void hideAchievementsContainer(int fadeDuration);
	void setSelectedAchievement(ACHIEVEMENT a);

	void hideAll(int fadeDuration);

	void keyPress(char c);

	void refreshUsers();
protected:
private:
	GUIContainer* mainContainer_;

	GUIContainer* optionsContainer_;

	GUIContainer* campaignContainer_;

	GUIContainer* creditsContainer_;

	PromptBox* IPEntryBox_;
	PromptBox* localUsernameEntryBox_;

	GUIContainer* loginEntryContainer_;
	GUIButton* loginCancelButton_;
	GUIButton* loginCreateButton_;
	GUIButton* loginSubmitButton_;
	GUITextbox* loginUsername_;
	GUITextbox* loginPassword_;

	GUIContainer* localLoginContainer_;
	GUIContainer* saveContainer_;
	UserDisplay* user1Container_;
	UserDisplay* user2Container_;
	UserDisplay* user3Container_;
	GUIButton* localLoginBackButton_;

	GUIContainer* achievementsContainer_;
	GUIContainer* achievementsDisplayContainer_;
	GUITexture* achievementTitle_;
	GUITexture* achievementDescription_;
	GUITexture* achievementIcon_;
};