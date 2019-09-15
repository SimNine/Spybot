#pragma once

#include "Standard.h"
#include "LinkedList.h"

struct Message;
class GameMirror;
class PlayerMirror;
class ClientMirror;

class Client {
public:
	Client();
	virtual ~Client();

	void processMessage(Message* m);
	void processAllMessages();

	PlayerMirror* getPlayer();
	void setPlayer(PlayerMirror* player);

	GameMirror* getGame();
	void setGame(GameMirror* game);

	int getClientID();
	void setClientID(int clientID);

	ClientMirror* getMyClientMirror();
	void setMyClientMirror(ClientMirror* mirr);
protected:

private:
	// the ID number of this client
	int myClientID_;

	// game entity and player entity (both NULL when no game is being played)
	GameMirror* game_;
	PlayerMirror* player_;

	// clientmirror entity
	ClientMirror* myClientMirror_;
};