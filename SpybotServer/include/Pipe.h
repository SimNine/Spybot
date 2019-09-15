#pragma once

#include "Standard.h"

class Server;
struct Message;
class Player;
class User;

class Pipe {
public:
	Pipe(SOCKET socket, Server* server);
	virtual ~Pipe();

	void listenData();
	void sendData(Message m);

	int getClientID();

	User* getUser();
	void setUser(User* user);

	Player* getPlayer();
	void setPlayer(Player* p);

	void close();
	bool isClosed();
protected:

private:
	SOCKET socket_;
	Server* server_;
	int clientID_;

	// the user that the client is currently logged in as
	User* user_;

	Player* player_;

	bool closed_;
};