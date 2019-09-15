#pragma once

#define NUM_CLIENT_IDS 128

#include "Standard.h"
#include "GUIContainer.h"

class ChatDisplay;
class GUIButton;

class LobbyOverlay : public GUIContainer {
public:
	LobbyOverlay();
	virtual ~LobbyOverlay();

	void draw();
	void tick(int ms);
	void resetBounds();

	void leaveLobby();

	ChatDisplay* getChatDisplay();

	void setGameMode(GAMEMODE gameMode);
protected:
private:
	void buildGUI();

	GUIContainer* gameConfigContainer_;

	GUIContainer* gameConfigLevelSelected_;
	GUIButton* gameConfigLevelArray_;
	GUIButton* gameConfigLevelHash_;
	GUIButton* gameConfigLevelCross_;
	GUIButton* gameConfigLevelShowdown_;

	GUIContainer* gameConnectedPlayerContainer_;
	GUIButton* gameConfigButtonCoop_;
	GUIButton* gameConfigButtonFFA_;
	GUIButton* gameConfigButtonTeamDM_;
	GUIButton* gameConfigButtonSelected_;

	ChatDisplay* chatDisplay_;
};