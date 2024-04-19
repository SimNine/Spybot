#pragma once

#include "Standard.h"
#include "PipeServerside.h"

struct Message;
class Player;
class User;

class PipeClientside {
public:
	PipeClientside(PipeServerside* socket);
	virtual ~PipeClientside();

	void sendData(Message m);
	void listenData();
	void recieveData(Message m);

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

	// the user that the client is currently logged in as
	std::string user_;
	int playerID_;

	bool closed_;
};