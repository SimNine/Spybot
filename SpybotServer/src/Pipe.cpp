#include "Standard.h"
#include "Pipe.h"

#include "Message.h"
#include "Server.h"
#include "MiscUtil.h"
#include "User.h"

Pipe::Pipe(SOCKET client, Server* server) {
	// fill in fields
	socket_ = client;
	server_ = server;
	player_ = NULL;
	closed_ = false;
	user_ = NULL;
	tempUser_ = NULL;
	newUser_ = NULL;

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

void Pipe::listenData() {
	int bytesRead;

	char readBuffer[DEFAULT_MSGSIZE];
	int readBufferLength = DEFAULT_MSGSIZE;

	// listen for connect message (this prevents non-game clients from connecting)
	bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
	if (bytesRead > 0) {
		Message m = deserializeMessage(readBuffer);
		if (m.type != MSGTYPE_CONNECT) {
			printf("SERVER: first message from clientID %i wasn't CONNECT message; likely not a game client. disconnecting...\n", clientID_);
			closed_ = true;
		} else {
			printf("SERVER: received confirmation CONNECT message from clientID %i\n", clientID_);
		}
	} else if (bytesRead == 0) {
		closed_ = true;
		printf("SERVER: received shutdown from clientID %u\n", clientID_);
	} else {
		closed_ = true;
		printf("SERVER ERR: recieve from clientID %u failed with error: %d\n", clientID_, WSAGetLastError());
	}

	// receive until the peer shuts down the connection
	while (!closed_) {
		bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
		if (bytesRead > 0) {
			Message m = deserializeMessage(readBuffer);
			server_->recieveMessage(m);
		} else if (bytesRead == 0) {
			closed_ = true;
			printf("SERVER: received shutdown from clientID %u\n", clientID_);
		} else {
			closed_ = true;
			printf("SERVER ERR: recieve from clientID %u failed with error: %d\n", clientID_, WSAGetLastError());
		}
	}

	server_->disconnect(this);

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
	serializeMessage(buffer, m);

	// send the message
	int bytesSent = send(socket_, buffer, DEFAULT_MSGSIZE, 0);
	if (bytesSent == SOCKET_ERROR) {
		printf("SERVER ERR: send to clientID %u failed with error: %d\n", clientID_, WSAGetLastError());
		closesocket(socket_);
	}
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

User* Pipe::getTempUser() {
	return tempUser_;
}

void Pipe::setTempUser(User* user) {
	tempUser_ = user;
}

User* Pipe::getNewUser() {
	return newUser_;
}

void Pipe::setNewUser(User* user) {
	newUser_ = user;
}