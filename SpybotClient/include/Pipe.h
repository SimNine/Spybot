#ifndef PIPE_H
#define PIPE_H

#include "Standard.h"

class Server;
struct Message;
class Player;
class User;

class Pipe {
public:
	Pipe(SOCKET socket, Server* server);
	virtual ~Pipe();

	void sendData(Message m);
	void listenData();

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

#endif // PIPE_H
