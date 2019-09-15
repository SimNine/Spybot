#ifndef CLIENT_H
#define CLIENT_H

#include "Standard.h"
#include "LinkedList.h"

struct Message;
class Game;
class Player;
class ClientMirror;

class Client {
public:
	Client();
	virtual ~Client();

	void processMessage(Message* m);
	void processAllMessages();

	Player* getPlayer();
	void setPlayer(Player* player);

	Game* getGame();
	void setGame(Game* game);

	int getClientID();
	void setClientID(int clientID);

protected:

private:
	// the ID number of this client
	int myClientID_;

	// game entity (NULL when no game is being played)
	Game* game_;
	Player* player_;
};

#endif // SERVER_H
