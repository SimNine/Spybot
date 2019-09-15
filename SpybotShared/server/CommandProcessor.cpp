#include "Standard.h"
#include "CommandProcessor.h"

#include <iostream>

#include "LinkedList.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Server.h"
#include "Message.h"
#include "Pipe.h"
#include "User.h"
#include "Player.h"
#include "Team.h"
#include "Game.h"

void processCommandResponse(std::string message, int clientID) {
	if (clientID == 0) {
		printf(">> %s\n", message.c_str());
	} else {
		Message m;
		m.type = MSGTYPE_CHAT;
		strcpy_s(m.text, DEFAULT_CHATSIZE, message.c_str());
		_server->sendMessageToClient(m, clientID);
	}
}

void processCommand(std::string cmd, int clientID) {
	// tokenize
	// (gotta add a newline because that's how the tokenizer works)
	char* tokens[1024];
	int numTokens = tokenize(tokens, (cmd + "\n").c_str(), ' ');

	// process tokens
	if (numTokens == 0) {
		processCommandResponse("no command entered", clientID);
	} else if (strcmp(tokens[0], "getowner") == 0) {
		Pipe* ownerClient = _server->getOwner();
		if (ownerClient == NULL)
			processCommandResponse("there is no owner client", clientID);
		else {
			std::string str = "the owner client is " + ownerClient->getClientID();
			processCommandResponse(str, clientID);
			if (ownerClient->getUser() != NULL) {
				std::string loginStr = " <logged in as " + ownerClient->getUser()->username_ + ">";
				processCommandResponse(loginStr, clientID);
			}
		}
	} else if (strcmp(tokens[0], "listclients") == 0) {
		processCommandResponse("connected clientIDs:", clientID);
		Iterator<Pipe*> it = _server->getClientList()->getIterator();
		while (it.hasNext()) {
			Pipe* curr = it.next();
			std::string str = "" + curr->getClientID();
			processCommandResponse(str, clientID);
			if (curr->getUser() != NULL) {
				std::string clientStr = "<logged in as " + curr->getUser()->username_ + ">";
				processCommandResponse(clientStr, clientID);
			} else {
				processCommandResponse("<not logged in>", clientID);
			}
		}
	} else if (strcmp(tokens[0], "kick") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: kick (clientID)", clientID);
			processCommandResponse("kick (clientID)", clientID);
			destroyTokens(tokens);
			return;
		}

		int clientID = atoi(tokens[1]);
		if (clientID == 0) {
			processCommandResponse("cannot kick clientID 0: is server", clientID);
			destroyTokens(tokens);
			return;
		}
		std::string str = "attempting to kick clientID " + clientID;
		processCommandResponse(str, clientID);

		Message m;
		m.type = MSGTYPE_LEAVE;
		m.clientID = clientID;
		_server->recieveMessage(m);
	} else if (strcmp(tokens[0], "load") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: load (levelnum)", clientID);
			processCommandResponse("load (levelnum)", clientID);
			destroyTokens(tokens);
			return;
		}

		int levelNum = atoi(tokens[1]);

		Message m;
		m.type = MSGTYPE_LOAD;
		m.levelNum = levelNum;
		m.clientID = 0;
		_server->recieveMessage(m);
	} else if (strcmp(tokens[0], "help") == 0) {
		processCommandResponse("Available commands:", clientID);
		processCommandResponse("listclients", clientID);
		processCommandResponse("kick (clientID)", clientID);
		processCommandResponse("load (levelnum)", clientID);
		processCommandResponse("say (string)", clientID);
		processCommandResponse("listplayers", clientID);
		processCommandResponse("adduser (username) (password)", clientID);
		processCommandResponse("deluser (username)", clientID);
		processCommandResponse("listusers", clientID);
		processCommandResponse("getowner", clientID);
	} else if (strcmp(tokens[0], "say") == 0) {
		std::string ret = "";
		for (int i = 1; i < numTokens; i++)
			ret += std::string(tokens[i]) + " ";

		Message m;
		m.type = MSGTYPE_CHAT;
		m.clientID = 0;
		strncpy_s(m.text, DEFAULT_CHATSIZE, ret.c_str(), DEFAULT_CHATSIZE);
		_server->recieveMessage(m);
	} else if (strcmp(tokens[0], "listplayers") == 0) {
		if (_server->getGame() == NULL) {
			processCommandResponse("cannot list players - no game currently in session", clientID);
			destroyTokens(tokens);
			return;
		}

		Iterator<Team*> itTeam = _server->getGame()->getAllTeams()->getIterator();
		while (itTeam.hasNext()) {
			Team* currTeam = itTeam.next();
			std::string str = "Team " + currTeam->getTeamNum();
			processCommandResponse(str, clientID);

			Iterator<Player*> itPlayer = currTeam->getAllPlayers()->getIterator();
			while (itPlayer.hasNext()) {
				Player* pCurr = itPlayer.next();
				if (pCurr->getMind() == NULL) {
					std::string str2 = "Player " + std::string(pCurr->getPlayerID() + " - Human");
					processCommandResponse(str2, clientID);
				} else {
					std::string str2 = "Player " + std::string(pCurr->getPlayerID() + " - AI");
					processCommandResponse(str2, clientID);
				}
			}
		}
	} /*else if (strcmp(tokens[0], "adduser") == 0) {
		// TODO: refactor this to use messages
		if (numTokens != 3) {
			printf(">> wrong number of arguments:\n");
			printf(">> adduser (username) (password)\n");
			destroyTokens(tokens);
			return;
		}

		User* u = new User();
		u->username_ = std::string(tokens[1]);
		u->password_ = std::string(tokens[2]);

		if (_server->getUserByName(u->username_) == NULL) {
			_server->getUsers()->addFirst(u);
			saveUsers();
			printf(">> successfully created user %s with password %s\n", tokens[1], tokens[2]);
		} else {
			delete u;
			printf(">> user creation failed\n");
			printf(">> a user with name \"%s\" already exists\n", u->username_.c_str());
		}
	} else if (strcmp(tokens[0], "deluser") == 0) {
		// TODO: refactor this to use messages
		if (numTokens != 2) {
			printf(">> wrong number of arguments:\n");
			printf(">> deluser (username)\n");
			destroyTokens(tokens);
			return;
		}

		User* u = getUserByName(std::string(tokens[1]));
		if (u == NULL)
			printf(">> user does not exist\n");
		else {
			_server->getUsers()->remove(u);
			delete u;
			saveUsers();
			printf(">> successfully deleted user %s\n", tokens[1]);
		}
	}*/ else if (strcmp(tokens[0], "listusers") == 0) {
		// TODO: refactor this to use messages
		processCommandResponse("Users:", clientID);
		Iterator<User*> it = _server->getUsers()->getIterator();
		while (it.hasNext()) {
			User* u = it.next();
			std::string str = u->username_ + " : " + u->password_;
			processCommandResponse(str, clientID);
		}
	} else {
		std::string str = "\"" + cmd + "\" is not a valid command";
		processCommandResponse(str, clientID);
		processCommandResponse("use \"help\" for a list of commands", clientID);
	}

	// destroy tokens
	destroyTokens(tokens);
}

void processCommandLoop() {
	while (true) {
		// get the line
		std::string buffer;
		getline(std::cin, buffer);

		// process it
		processCommand(buffer, 0);
	}

	printf("SERVER: parser loop existed for some reason\n");
}