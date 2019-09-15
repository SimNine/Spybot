#pragma once

#include "Standard.h"
#include "LinkedList.h"

class ClientMirror;
struct Message;
class Server;
class Pipe;

class ConnectionManager {
public:
	ConnectionManager();
	virtual ~ConnectionManager();

	void connectToExternalServer(std::string IP);
	void connectToLocalServer(CAMPAIGN campaign);
	void disconnect();
	void listen();

	void sendMessage(Message message);
	void recieveMessage(Message message);
	Message* pollMessage();
	bool hasMessage();

	ClientMirror* getClientMirrorByClientID(int clientID);
	ClientMirror* getClientMirrorByPlayerID(int playerID);
	LinkedList<ClientMirror*>* getClientList();
	ClientMirror* getServerOwner();
	void setServerOwner(ClientMirror* clientMirror);
	
	void pingChecker();
	void resetPingCount();
protected:

private:
	// for keeping track of server survival
	int pingCount_ = 0;

	// for external server interation
	SOCKET socket_;

	// for internal server interaction
	Pipe* serverPipe_;

	// message passing utils
	LinkedList<Message*>* msgQueue_;
	std::mutex msgMutex_;

	// all clients connected to server (null if no connection)
	LinkedList<ClientMirror*>* clientList_;
	ClientMirror* serverOwner_;
};