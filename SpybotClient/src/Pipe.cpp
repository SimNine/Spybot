#include "Standard.h"
#include "Pipe.h"

#include "Global.h"
#include "Message.h"
#include "Server.h"
#include "MiscUtil.h"
#include "User.h"
#include "Client.h"
#include "ConnectionManager.h"

Pipe::Pipe(SOCKET client, Server* server) {
	// fill in fields
	socket_ = client;
	server_ = server;
	player_ = NULL;
	closed_ = false;
	user_ = NULL;

	// set client ID
	clientID_ = randInt();

	// send a message informing this client about its new ID
	Message m;
	m.type = MSGTYPE_CONNECT;
	m.clientID = clientID_;
	this->sendData(m);
	printf("SERVER: new client attempting to connect, assigning clientID %u\n", clientID_);
}

Pipe::~Pipe() {
	closed_ = true;
	if (user_ != NULL)
		user_->loggedIn_ = false;
}

void Pipe::sendData(Message m) {
	_connectionManager->recieveMessage(m);
}

void Pipe::listenData() {
	// this purposefully does nothing for local servers
}

int Pipe::getClientID() {
	return clientID_;
}

Player* Pipe::getPlayer() {
	return player_;
}

void Pipe::setPlayer(Player* p) {
	player_ = p;
}

void Pipe::close() {
	closed_ = true;
	shutdown(socket_, SD_SEND);
	closesocket(socket_);
}

bool Pipe::isClosed() {
	return closed_;
}

User* Pipe::getUser() {
	return user_;
}

void Pipe::setUser(User* user) {
	user_ = user;
}