#include "Standard.h"
#include "Main.h"

#include "Message.h"
#include "Server.h"
#include "Pipe.h"

Server* server;
DEBUG debug = DEBUG_NORMAL;

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

// command line function
void parseCommands()
{
	while (true)
	{
		// get the line
		std::string buffer;
		getline(std::cin, buffer);

		// tokenize
		// (gotta add a newline because that's how the tokenizer works)
		char* tokens[1024];
		int numTokens = tokenize(tokens, (buffer + "\n").c_str(), ' ');
		
		// process tokens
		if (numTokens == 0)
		{
			printf(">> no command entered\n");
		}
		else if (strcmp(tokens[0], "listclients") == 0)
		{
			printf(">> connected clientIDs:\n");
			LinkedList<Pipe*>* players = server->getClientList();
			Iterator<Pipe*> it = players->getIterator();
			while (it.hasNext())
			{
				Pipe* curr = it.next();
				printf(">> %i\n", curr->getClientID());
			}
		}
		else if (strcmp(tokens[0], "kick") == 0)
		{
			if (numTokens != 2)
			{
				printf(">> wrong number of arguments:\nkick (clientID)\n");
				printf(">> kick (clientID)\n");
				destroyTokens(tokens);
				continue;
			}

			int clientID = atoi(tokens[1]);
			if (clientID == 0)
			{
				printf(">> cannot kick clientID 0: is server\n");
				destroyTokens(tokens);
				continue;
			}
			printf(">> attempting to kick clientID %i\n", clientID);

			Message m;
			m.type = MSGTYPE_LEAVE;
			m.clientID = clientID;
			server->recieveMessage(m);
		}
		else if (strcmp(tokens[0], "load") == 0)
		{
			if (numTokens != 2)
			{
				printf(">> wrong number of arguments:\nload (levelnum)\n");
				printf(">> load (levelnum)\n");
				destroyTokens(tokens);
				continue;
			}

			int levelNum = atoi(tokens[1]);

			Message m;
			m.type = MSGTYPE_LOAD;
			m.levelNum = levelNum;
			m.clientID = 0;
			server->recieveMessage(m);
		}
		else if (strcmp(tokens[0], "help") == 0)
		{
			printf(">> Available commands:\n");
			printf(">> listclients\n");
			printf(">> kick (clientID)\n");
			printf(">> load (levelnum)\n");
		}
		else
		{
			printf(">> \"%s\" is not a valid command\n", buffer.c_str());
			printf(">> use \"help\" for a list of commands\n");
		}

		// destroy tokens
		destroyTokens(tokens);
	}

	printf("parser loop existed for some reason\n");
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
	std::thread commandLine(parseCommands);
	commandLine.detach();
	printf("Command line parser set up and detached\n");

	// tick the server continuously
	while (true)
	{
		server->tick(0);
	}

	return 0;
}

int tokenize(char** tokens, const char* string, char delimiter)
{
	char buffer[1024]; // array of characters to make a string of
	int currToken = 0;
	int currPos = 0;

	for (int i = 0; i < strlen(string); i++)
	{
		if ((string[i] == delimiter || string[i] == '\n') && currPos == 0)
		{
			continue;
		}
		else if (string[i] == delimiter || string[i] == '\n' || i == strlen(string))
		{
			buffer[currPos] = '\0';
			tokens[currToken] = (char*)calloc(currPos, (sizeof(char) * strlen(buffer)) + 1);

			for (int j = 0; j < strlen(buffer); j++)
			{
				tokens[currToken][j] = buffer[j];
			}

			currToken++;
			currPos = 0;
		}
		else
		{
			buffer[currPos] = string[i];
			currPos++;
		}
	}

	// end the array
	if (currPos == 0)
	{
		tokens[currToken] = NULL;
		return currToken;
	}
	else
	{
		tokens[currToken + 1] = NULL;
		return currToken + 1;
	}
}

void destroyTokens(char** tokens) 
{
	for (int i = 0; ; i++) 
	{
		if (tokens[i] == NULL) 
		{
			break;
		}
		else 
		{
			free(tokens[i]);
			tokens[i] = NULL;
		}
	}
}