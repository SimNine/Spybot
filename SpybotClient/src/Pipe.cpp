#include "Standard.h"
#include "Pipe.h"

#include "Global.h"
#include "Message.h"
#include "Server.h"
#include "MiscUtil.h"
#include "User.h"
#include "Client.h"
#include "ConnectionManager.h"

Pipe::Pipe(SOCKET client) {
	// fill in fields
	socket_ = client;
	closed_ = false;
	playerID_ = -1;
	user_ = "";

	// set client ID
	clientID_ = randInt();

	// send a message informing this client about its new ID
	Message m;
	m.type = MSGTYPE_CONNECT;
	this->sendData(m);
	log("SERVER: new client attempting to connect, assigning clientID " + to_string(clientID_) + "\n");
}

Pipe::~Pipe() {
	closed_ = true;
	//if (user_ != "")
		//user_->loggedIn_ = false;
}

void Pipe::sendData(Message m) {
	m.clientID = clientID_;
	_connectionManager->recieveMessage(m);
}

void Pipe::listenData() {
	// this purposefully does nothing for local servers
}

void Pipe::recieveData(Message m) {
	m.clientID = clientID_;
	_server->recieveMessage(m);
}

int Pipe::getClientID() {
	return clientID_;
}

int Pipe::getPlayer() {
	return playerID_;
}

void Pipe::setPlayer(int playerID) {
	playerID_ = playerID;

	Message m;
	m.type = MSGTYPE_SETCLIENTPLAYER;
	m.clientID = clientID_;
	m.playerID = playerID;
	_connectionManager->recieveMessage(m);
}

void Pipe::close() {
	closed_ = true;
	shutdown(socket_, SD_SEND);
	closesocket(socket_);
}

bool Pipe::isClosed() {
	return closed_;
}

std::string Pipe::getUser() {
	return user_;
}

void Pipe::setUser(std::string user) {
	user_ = user;
}