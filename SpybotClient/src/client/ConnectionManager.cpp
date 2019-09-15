#include "Standard.h"
#include "ConnectionManager.h"

#include "Message.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Data.h"
#include "ClientMirror.h"
#include "PlayerMirror.h"
#include "Server.h"
#include "Client.h"

ConnectionManager::ConnectionManager() {
	msgQueue_ = new LinkedList<Message*>();
	clientList_ = new LinkedList<ClientMirror*>();
	serverOwner_ = NULL;
	socket_ = INVALID_SOCKET;
}

ConnectionManager::~ConnectionManager() {
	//dtor
}

void ConnectionManager::recieveMessage(Message message) {
	mtx_.lock();
	msgQueue_->addLast(new Message(message));
	mtx_.unlock();
}

Message* ConnectionManager::pollMessage() {
	mtx_.lock();
	Message* ret = msgQueue_->poll();
	mtx_.unlock();
	return ret;
}

bool ConnectionManager::hasMessage() {
	return (msgQueue_->getLength() > 0);
}

void ConnectionManager::connectToLocalServer(std::string savePath) {
	// disconnect in case there is still some connection
	disconnect();

	_server = new Server(true, savePath);
	_server->connect(NULL);
}

void ConnectionManager::connectToExternalServer(std::string IP) {
	// disconnect in case there is still some connection
	disconnect();

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	int iResult;
	struct addrinfo *result = NULL;
	iResult = getaddrinfo(IP.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		log("CLIENT ERR: getaddrinfo failed with error: " + to_string(iResult) + "\n");
	}

	// Attempt to connect to an address until one succeeds
	struct addrinfo* ptr = NULL;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		socket_ = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (socket_ == INVALID_SOCKET) {
			log("CLIENT ERR: socket failed with error: " + to_string(WSAGetLastError()) + "\n");
		}

		// Connect to server.
		iResult = connect(socket_, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(socket_);
			socket_ = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (socket_ == INVALID_SOCKET) {
		log("CLIENT ERR: Unable to connect to server!\n");
	}

	// Receive until the peer closes the connection
	log("CLIENT: attempting to connect to server \"" + IP + "\"\n");
	std::thread newThread(&ConnectionManager::listen, this);
	newThread.detach();
}

void ConnectionManager::disconnect() {
	if (socket_ != INVALID_SOCKET) { // disconnect from external server
		if (shutdown(socket_, SD_SEND) == SOCKET_ERROR) {
			log("CLIENT ERR: shutdown failed with error: " + to_string(WSAGetLastError()) + "\n");
		}
		closesocket(socket_);

		while (msgQueue_->getLength() > 0)
			delete msgQueue_->poll();
		while (clientList_->getLength() > 0)
			delete clientList_->poll();
		socket_ = INVALID_SOCKET;
	} else if (_server != NULL) { // disconnect from internal server
		delete _server;
		_server = NULL;
	}
	serverOwner_ = NULL;
	_client->setClientID(-1);
}

void ConnectionManager::listen() {
	int bytesRead;

	char readBuffer[DEFAULT_MSGSIZE];
	int readBufferLength = DEFAULT_MSGSIZE;

	// listen for the first message, which should be a connection confirmation
	bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
	if (bytesRead > 0) {
		Message m = _deserializeMessage(readBuffer);
		if (m.type == MSGTYPE_CONNECT) {
			_client->setClientID(m.clientID);
			//strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, username.c_str(), DEFAULT_MSG_TEXTSIZE);
			log("CLIENT: received connection confirmation from server, acquired client ID " + to_string(_client->getClientID()) + "\n");
			this->recieveMessage(m);
			this->sendMessage(m);
		} else {
			log("CLIENT ERR: first message recieved from server was not connection confirmation\n");
			disconnect();
			return;
		}
	} else if (bytesRead == 0) {
		log("CLIENT: recieved shutdown from server\n");
	} else {
		log("CLIENT ERR: recieve from server failed with error: " + to_string(WSAGetLastError()) + "\n");
	}

	// receive messages until the peer shuts down the connection or there's a read error
	do {
		bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
		if (bytesRead > 0) {
			Message m = _deserializeMessage(readBuffer);
			recieveMessage(m);
		} else if (bytesRead == 0)
			log("CLIENT: recieved shutdown from server\n");
		else {
			log("CLIENT ERR: recieve from server failed with error: " + to_string(WSAGetLastError()) + "\n");
		}
	} while (bytesRead > 0);

	disconnect();
	return;
}

void ConnectionManager::sendMessage(Message m) {
	m.clientID = _client->getClientID();
	if (socket_ != INVALID_SOCKET) {
		char buffer[DEFAULT_MSGSIZE];
		_serializeMessage(buffer, m);

		int bytesSent = send(socket_, buffer, DEFAULT_MSGSIZE, 0);
		if (bytesSent == SOCKET_ERROR) {
			log("CLIENT ERR: send failed with error: " + to_string(WSAGetLastError()) + "\n");
			closesocket(socket_);
		}
	} else if (_server != NULL) {
		_server->recieveMessage(m);
	}
}

LinkedList<ClientMirror*>* ConnectionManager::getClientList() {
	return clientList_;
}

ClientMirror* ConnectionManager::getClientMirrorByClientID(int clientID) {
	Iterator<ClientMirror*> it = clientList_->getIterator();
	while (it.hasNext()) {
		ClientMirror* curr = it.next();
		if (curr->clientID_ == clientID)
			return curr;
	}

	return NULL;
}

ClientMirror* ConnectionManager::getClientMirrorByPlayerID(int playerID) {
	Iterator<ClientMirror*> it = clientList_->getIterator();
	while (it.hasNext()) {
		ClientMirror* curr = it.next();
		if (curr->player_->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
}

ClientMirror* ConnectionManager::getServerOwner() {
	return serverOwner_;
}

void ConnectionManager::setServerOwner(ClientMirror* serverOwner) {
	serverOwner_ = serverOwner;
}