#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#define NUM_CLIENT_IDS 128

#include "Standard.h"
#include "GUIContainer.h"

class ChatDisplay;

class LobbyScreen : public GUIContainer
{
    public:
        LobbyScreen();
        virtual ~LobbyScreen();

		void draw();
		void leaveLobby();

		ChatDisplay* getChatDisplay();
    protected:
    private:
		void buildGUI();

		ChatDisplay* chatDisplay_;
};

#endif // LOBBYSCREEN_H
