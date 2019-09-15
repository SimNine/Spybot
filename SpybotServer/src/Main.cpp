#include "Standard.h"

#include "Global.h"
#include "Message.h"
#include "Server.h"
#include "Pipe.h"
#include "MiscUtil.h"
#include "CommandProcessor.h"

// main loop to recieve new sockets
void masterSocketFunc() {
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		log("WSAStartup failed with error: " + to_string(iResult) + "\n");
		exit(1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		log("getaddrinfo failed with error: " + to_string(iResult) + "\n");
		WSACleanup();
		exit(1);
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		log("socket failed with error: " + to_string(WSAGetLastError()) + "\n");
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		log("bind failed with error: " + to_string(WSAGetLastError()) + "\n");
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	freeaddrinfo(result);

	// listen for incoming connections
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		log("listen failed with error: " + to_string(WSAGetLastError()) + "\n");
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// main master listener loop
	while (true) {
		// attempt to accept a client socket
		SOCKET newSock = accept(ListenSocket, NULL, NULL);
		if (newSock == INVALID_SOCKET) {
			log("accept failed with error: " + to_string(WSAGetLastError()) + "\n");
			closesocket(ListenSocket);
			WSACleanup();
			exit(1);
		} else
			_server->connect(newSock);
	}
}

// main function
int main(int argc, char* args[]) {
	logInit("log-server.txt", false);
	log("\n");
	log("-----SpybotServer-----\n");

	// initialize the server
	_server = new Server(false, "levels/multi");
	log("Server initialized\n");

	// start up the master listener thread
	std::thread masterListener(masterSocketFunc);
	masterListener.detach();
	log("Master listener set up and detached\n");

	// start up the command line thread
	std::thread commandLine(processCommandLoop);
	commandLine.detach();
	log("Command line parser set up and detached\n");

	// tick the server continuously
	while (!_quit)
		_server->processAllMessages();

	return 0;
}