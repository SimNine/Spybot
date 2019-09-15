#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#define NUM_CLIENT_IDS 128

#include "Standard.h"
#include "GUIContainer.h"

class LobbyScreen : public GUIContainer
{
    public:
        LobbyScreen();
        virtual ~LobbyScreen();

		void draw();
		void leaveLobby();
    protected:
    private:
		void buildGUI();
};

#endif // LOBBYSCREEN_H
