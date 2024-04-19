#include "Standard.h"
#include "PipeClientside.h"

#include "Global.h"
#include "Message.h"
#include "Server.h"
#include "MiscUtil.h"
#include "User.h"
#include "Client.h"
#include "ConnectionManager.h"

PipeClientside::PipeClientside(PipeServerside* client) {
	// fill in fields
	socket_ = NULL;
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

PipeClientside::~PipeClientside() {
	closed_ = true;
	//if (user_ != "")
		//user_->loggedIn_ = false;
}

void PipeClientside::sendData(Message m) {
	m.clientID = clientID_;
	_connectionManager->recieveMessage(m);
}

void PipeClientside::listenData() {
	// this purposefully does nothing for local servers
}

void PipeClientside::recieveData(Message m) {
	m.clientID = clientID_;
	_server->recieveMessage(m);
}

int PipeClientside::getClientID() {
	return clientID_;
}

int PipeClientside::getPlayer() {
	return playerID_;
}

void PipeClientside::setPlayer(int playerID) {
	playerID_ = playerID;

	Message m;
	m.type = MSGTYPE_SETCLIENTPLAYER;
	m.clientID = clientID_;
	m.playerID = playerID;
	_connectionManager->recieveMessage(m);
}

void PipeClientside::close() {
	closed_ = true;
	shutdown(socket_, SD_SEND);
	closesocket(socket_);
}

bool PipeClientside::isClosed() {
	return closed_;
}

std::string PipeClientside::getUser() {
	return user_;
}

void PipeClientside::setUser(std::string user) {
	user_ = user;
}