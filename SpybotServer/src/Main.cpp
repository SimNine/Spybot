#include "Standard.h"
#include "Main.h"

#include "Message.h"
#include "Server.h"
#include "Pipe.h"
#include "MiscUtil.h"

Server* server;
DEBUG _debug = DEBUG_NORMAL;

// main loop to recieve new sockets
void masterSocketFunc()
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
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
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	freeaddrinfo(result);

	// listen for incoming connections
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// main master listener loop
	while (true)
	{
		// attempt to accept a client socket
		SOCKET newSock = accept(ListenSocket, NULL, NULL);
		if (newSock == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			exit(1);
		}
		else
			server->connect(newSock);
	}
}

// main function
int main(int argc, char* args[])
{
	printf("SpybotServer.exe launched\n");

	// initialize the server
	server = new Server();
	printf("Server initialized\n");

	// start up the master listener thread
	std::thread masterListener(masterSocketFunc);
	masterListener.detach();
	printf("Master listener set up and detached\n");

	// start up the command line thread
	std::thread commandLine(&Server::processCommandLoop, server);
	commandLine.detach();
	printf("Command line parser set up and detached\n");

	// tick the server continuously
	while (true)
		server->tick(0);

	return 0;
}