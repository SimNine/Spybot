#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "Standard.h"
#include "LinkedList.h"

class ClientMirror;
struct Message;
class Server;

class ConnectionManager {
public:
	ConnectionManager();
	virtual ~ConnectionManager();

	void connectToExternalServer(std::string IP);
	void connectToLocalServer(std::string savePath);
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
protected:

private:
	// for external server interation
	SOCKET socket_;

	// message passing utils
	LinkedList<Message*>* msgQueue_;
	std::mutex mtx;

	// all clients connected to server (null if no connection)
	LinkedList<ClientMirror*>* clientList_;
	ClientMirror* serverOwner_;
};

#endif // CONNECTIONMANAGER_H
