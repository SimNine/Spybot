#pragma once

#include "Standard.h"

struct Message;
class Player;
class User;

class Pipe {
public:
	Pipe(SOCKET socket);
	virtual ~Pipe();

	void listenData();
	void sendData(Message m);

	int getClientID();

	std::string getUser();
	void setUser(std::string user);

	int getPlayer();
	void setPlayer(int playerID);

	void close();
	bool isClosed();
protected:

private:
	SOCKET socket_;
	int clientID_;

	std::string user_;
	int playerID_;

	bool closed_;
};