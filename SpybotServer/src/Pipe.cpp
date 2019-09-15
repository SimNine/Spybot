#include "Standard.h"
#include "Pipe.h"

#include "Message.h"
#include "Server.h"
#include "MiscUtil.h"
#include "User.h"
#include "Global.h"

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
	m.clientID = clientID_;
	this->sendData(m);
	log("SERVER: new client attempting to connect, assigning clientID " + to_string(clientID_) + "\n");
}

Pipe::~Pipe() {
	closed_ = true;
	if (user_ == "")
		_server->getUserByName(user_)->loggedIn_ = false;
}

void Pipe::listenData() {
	int bytesRead;

	char readBuffer[DEFAULT_MSGSIZE];
	int readBufferLength = DEFAULT_MSGSIZE;

	// listen for connect message (this prevents non-game clients from connecting)
	bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
	if (bytesRead > 0) {
		Message m = _deserializeMessage(readBuffer);
		if (m.type != MSGTYPE_CONNECT) {
			log("SERVER: first message from clientID " + to_string(clientID_) + " wasn't CONNECT message; likely not a game client. disconnecting...\n");
			closed_ = true;
		} else {
			log("SERVER: received confirmation CONNECT message from clientID " + to_string(clientID_) + "\n");
		}
	} else if (bytesRead == 0) {
		closed_ = true;
		log("SERVER: received shutdown from clientID " + to_string(clientID_) + "\n");
	} else {
		closed_ = true;
		log("SERVER ERR: recieve from clientID " + to_string(clientID_) + " failed with error: " + to_string(WSAGetLastError()) + "\n");
	}

	// receive until the peer shuts down the connection
	while (!closed_) {
		bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
		if (bytesRead > 0) {
			Message m = _deserializeMessage(readBuffer);
			m.clientID = clientID_;
			_server->recieveMessage(m);
		} else if (bytesRead == 0) {
			closed_ = true;
			log("SERVER: received shutdown from clientID " + to_string(clientID_) + "\n");
		} else {
			closed_ = true;
			log("SERVER ERR: recieve from clientID " + to_string(clientID_) + " failed with error: " + to_string(WSAGetLastError()) + "\n");
		}
	}

	_server->disconnect(this);

	// shutdown and socket closure should have already happened, but if they didn't, might as well do it again
	shutdown(socket_, SD_SEND);
	closesocket(socket_);
	delete this;
	return;
}

void Pipe::sendData(Message m) {
	// create the buffer to serialize the message into
	char buffer[DEFAULT_MSGSIZE];

	// serialize the message into the buffer
	_serializeMessage(buffer, m);

	// send the message
	int bytesSent = send(socket_, buffer, DEFAULT_MSGSIZE, 0);
	if (bytesSent == SOCKET_ERROR) {
		log("SERVER ERR: send to clientID " + to_string(clientID_) + " failed with error: " + to_string(WSAGetLastError()) + "\n");
		_server->disconnect(this);

		// shutdown and socket closure should have already happened, but if they didn't, might as well do it again
		shutdown(socket_, SD_SEND);
		closesocket(socket_);
		delete this;
	}
}

int Pipe::getClientID() {
	return clientID_;
}

int Pipe::getPlayer() {
	return playerID_;
}

void Pipe::setPlayer(int p) {
	playerID_ = p;
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