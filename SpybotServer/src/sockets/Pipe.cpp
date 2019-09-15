#include "Standard.h"
#include "Pipe.h"

#include "Message.h"
#include "Server.h"
#include "MiscUtil.h"

Pipe::Pipe(SOCKET client, Server* server)
{
	// fill in fields
    socket_ = client;
    server_ = server;
	player_ = NULL;
	dead_ = false;

	// set client ID
	clientID_ = randInt();

	// send a message informing this client about its new ID
	Message m;
	m.type = MSGTYPE_CONNECT;
	m.clientID = clientID_;
	this->sendData(m);
	printf("SERVER: new client attempting to connect, assigning clientID %u\n", clientID_);
}

Pipe::~Pipe()
{
	dead_ = true;
}

void Pipe::listenData()
{
	int bytesRead;

	char readBuffer[DEFAULT_MSGSIZE];
	int readBufferLength = DEFAULT_MSGSIZE;

	// listen for connect message (this prevents non-game clients from connecting)
	bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
	if (bytesRead > 0) 
	{
		Message m = deserializeMessage(readBuffer);
		if (m.type != MSGTYPE_CONNECT)
		{
			printf("SERVER: first message from clientID %i wasn't CONNECT message; likely not a game client. disconnecting...\n", clientID_);
			dead_ = true;
		}
		else
		{
			printf("SERVER: received initial CONNECT message from clientID %i - %s\n", clientID_, m.text);
			server_->recieveMessage(m);
		}
	}
	else if (bytesRead <= 0)
	{
		dead_ = true;
		printf("SERVER: received shutdown from clientID %u\n", clientID_);
	}
	else 
	{
		dead_ = true;
		printf("SERVER ERR: recieve from clientID %u failed with error: %d\n", clientID_, WSAGetLastError());
	}

	// receive until the peer shuts down the connection
	while (!dead_) {
		bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
		if (bytesRead > 0) 
		{
			Message m = deserializeMessage(readBuffer);
			server_->recieveMessage(m);
		}
		else if (bytesRead == 0)
		{
			dead_ = true;
			printf("SERVER: received shutdown from clientID %u\n", clientID_);
		}
		else 
		{
			dead_ = true;
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

void Pipe::sendData(Message m)
{
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

int Pipe::getClientID()
{
	return clientID_;
}

Player* Pipe::getPlayer()
{
	return player_;
}

void Pipe::setPlayer(Player* p)
{
	player_ = p;
}

void Pipe::kill()
{
	dead_ = true;
	shutdown(socket_, SD_SEND);
	closesocket(socket_);
}

bool Pipe::isDead()
{
	return dead_;
}

std::string Pipe::getName()
{
	return name_;
}

void Pipe::setName(std::string name)
{
	name_ = name;
}