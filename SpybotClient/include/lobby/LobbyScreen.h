#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#define NUM_CLIENT_IDS 128

#include "Standard.h"
#include "GUIContainer.h"

class ChatDisplay;
class GUIButton;

class LobbyScreen : public GUIContainer
{
    public:
        LobbyScreen();
        virtual ~LobbyScreen();

		void draw();
		void tick(int ms);
		void resetBounds();

		void leaveLobby();

		ChatDisplay* getChatDisplay();

		void setGameMode(GAMEMODE gameMode);
    protected:
    private:
		GAMEMODE gameMode_;

		void buildGUI();

		GUIContainer* gameConfigContainer_;
		GUIButton* gameConfigButtonCoop_;
		GUIButton* gameConfigButtonFFA_;
		GUIButton* gameConfigButtonTeamDM_;
		GUIButton* gameConfigButtonSelected_;

		ChatDisplay* chatDisplay_;
};

#endif // LOBBYSCREEN_H
