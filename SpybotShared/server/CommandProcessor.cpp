#include "Standard.h"
#include "CommandProcessor.h"

#include <iostream>

#include "LinkedList.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Server.h"
#include "Message.h"
#include "PipeServerside.h"
#include "User.h"
#include "Player.h"
#include "Team.h"
#include "Game.h"
#include "Program.h"
#include "SpawnGroup.h"

void processCommandResponse(std::string message, int clientID) {
	if (clientID == 0) {
		log(">> " + message + "\n");
	} else {
		Message m;
		m.type = MSGTYPE_CHAT;
		strcpy_s(m.text, DEFAULT_CHATSIZE, message.c_str());
		_server->sendMessageToClient(m, clientID);
	}
}

void processCommand(std::string cmd, int clientID) {
	// tokenize (gotta add a newline because that's how the tokenizer works)
	char* tokens[1024];
	int numTokens = tokenize(tokens, (cmd + "\n").c_str(), ' ');

	// process tokens
	if (numTokens == 0) {
		processCommandResponse("no command entered", clientID);
	} else if (strcmp(tokens[0], "help") == 0) {
		processCommandResponse("Available commands:", clientID);
		processCommandResponse("-----", clientID);

		// user manipulation
		//processCommandResponse("adduser (username) (password)", clientID);
		//processCommandResponse("deluser (username)", clientID);
		processCommandResponse("listusers", clientID);
		processCommandResponse("userinfo (username)", clientID);
		processCommandResponse("userinventory (username)", clientID);
		processCommandResponse("-----", clientID);

		// client and player manipulation
		processCommandResponse("listclients", clientID);
		processCommandResponse("kick (clientID)", clientID);
		processCommandResponse("say (string)", clientID);
		processCommandResponse("listplayers", clientID);
		processCommandResponse("getowner", clientID);
		processCommandResponse("-----", clientID);

		// team manipulation
		processCommandResponse("addteam", clientID);
		processCommandResponse("removeteam (teamID)", clientID);
		processCommandResponse("-----", clientID);

		// spawn group manipulation
		processCommandResponse("addspawngroup", clientID);
		processCommandResponse("removespawngroup (groupID)", clientID);
		processCommandResponse("listspawngroups", clientID);
		processCommandResponse("addspawngrouptile (groupID) (x) (y)", clientID);
		processCommandResponse("removespawngrouptile (groupID) (x) (y)", clientID);
		processCommandResponse("-----", clientID);

		// level manipulation
		processCommandResponse("startgame", clientID);
		processCommandResponse("save (filename)", clientID);
		processCommandResponse("settile (x) (y) (tiletype)", clientID);
		processCommandResponse("settilerect (x) (y) (width) (height) (tiletype)", clientID);
		processCommandResponse("-----", clientID);

		// printing and debugging
		processCommandResponse("printgridprograms", clientID);
		processCommandResponse("printgridspawngroups", clientID);
		processCommandResponse("-----", clientID);

		// misc
		processCommandResponse("giveprogram (user) (progtype) (count)", clientID);
		processCommandResponse("-----", clientID);
	}

	// USER MANIPULATION
	/*
	else if (strcmp(tokens[0], "adduser") == 0) {
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
	}
	*/
	else if (strcmp(tokens[0], "listusers") == 0) {
		// TODO: refactor this to use messages
		processCommandResponse("users:", clientID);
		Iterator<User*> it = _server->getUsers()->getIterator();
		while (it.hasNext()) {
			User* u = it.next();
			processCommandResponse("-- " + u->username_ + " : " + u->password_, clientID);
		}
	} else if (strcmp(tokens[0], "userinfo") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: userinfo (username)", clientID);
			destroyTokens(tokens);
			return;
		}

		User* u = _server->getUserByName(std::string(tokens[1]));
		if (u == NULL) {
			std::string str = "user \"" + std::string(tokens[1]) + "\" not found";
			processCommandResponse(str, clientID);
		} else {
			std::string str = "user \"" + std::string(tokens[1]) + "\" details:";
			processCommandResponse(str, clientID);
			str = (u->loggedIn_) ? "-- logged in: TRUE" : "-- logged in: FALSE";
			processCommandResponse(str, clientID);
			str = "-- credits: " + to_string(u->numCredits_);
			processCommandResponse(str, clientID);
		}
	} else if (strcmp(tokens[0], "userinventory") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: userinventory (username)", clientID);
			destroyTokens(tokens);
			return;
		}

		User* u = _server->getUserByName(std::string(tokens[1]));
		if (u == NULL) {
			std::string str = "user \"" + std::string(tokens[1]) + "\" not found";
			processCommandResponse(str, clientID);
		} else {
			for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
				std::string str = "-- " + to_string(i) + " : " + to_string(u->progsOwned_[i]);
				processCommandResponse(str, clientID);
			}
		}
	}

	// CLIENT AND PLAYER MANIPULATION 
	else if (strcmp(tokens[0], "listclients") == 0) {
		processCommandResponse("connected clientIDs:", clientID);
		Iterator<PipeServerside*> it = _server->getClientList()->getIterator();
		while (it.hasNext()) {
			PipeServerside* curr = it.next();
			if (curr->getUser() != "") {
				processCommandResponse("-- client " + to_string(curr->getClientID()) + " : logged in as " + curr->getUser(), clientID);
			} else {
				processCommandResponse("-- client " + to_string(curr->getClientID()) + " : not logged in", clientID);
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
		std::string str = "attempting to kick clientID " + to_string(clientID);
		processCommandResponse(str, clientID);

		Message m;
		m.type = MSGTYPE_LEAVE;
		m.clientID = clientID;
		_server->recieveMessage(m);
	} else if (strcmp(tokens[0], "say") == 0) {
		std::string ret = "";
		for (int i = 1; i < numTokens; i++)
			ret += std::string(tokens[i]) + " ";

		Message m;
		m.type = MSGTYPE_CHAT;
		m.clientID = 0;
		strncpy_s(m.text, DEFAULT_CHATSIZE, ret.c_str(), DEFAULT_CHATSIZE);

		Iterator<PipeServerside*> clientIt = _server->getClientList()->getIterator();
		while (clientIt.hasNext()) {
			clientIt.next()->sendData(m);
		}
	} else if (strcmp(tokens[0], "listplayers") == 0) {
		if (_server->getGame() == NULL) {
			processCommandResponse("cannot list players - no game currently in session", clientID);
			destroyTokens(tokens);
			return;
		}

		if (_server->getGame()->getAllTeams()->getLength() == 0) {
			processCommandResponse("there are no players", clientID);
			destroyTokens(tokens);
			return;
		}

		Iterator<Team*> itTeam = _server->getGame()->getAllTeams()->getIterator();
		while (itTeam.hasNext()) {
			Team* currTeam = itTeam.next();
			std::string str = "team " + to_string(currTeam->getTeamID()) + ":";
			processCommandResponse(str, clientID);

			Iterator<Player*> itPlayer = currTeam->getAllPlayers()->getIterator();
			while (itPlayer.hasNext()) {
				Player* pCurr = itPlayer.next();
				if (pCurr->getMind() == NULL) {
					std::string str2 = "-- player " + std::string(to_string(pCurr->getPlayerID()) + " : human");
					processCommandResponse(str2, clientID);
				} else {
					std::string str2 = "-- player " + std::string(to_string(pCurr->getPlayerID()) + " : AI");
					processCommandResponse(str2, clientID);
				}
			}
		}
	} else if (strcmp(tokens[0], "getowner") == 0) {
		PipeServerside* ownerClient = _server->getOwner();
		if (ownerClient == NULL) {
			processCommandResponse("there is no owner client", clientID);
		} else {
			std::string str = "the owner client is " + ownerClient->getClientID();
			processCommandResponse(str, clientID);
			if (ownerClient->getUser() != "") {
				std::string loginStr = to_string(ownerClient->getClientID()) + " <logged in as " + ownerClient->getUser() + ">";
				processCommandResponse(loginStr, clientID);
			}
		}
	}

	// TEAM MANIPULATION
	else if (strcmp(tokens[0], "addteam") == 0) {
		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		Team* team = g->addTeam();
		processCommandResponse("created new Team " + to_string(team->getTeamID()), clientID);
	} else if (strcmp(tokens[0], "removeteam") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: removeteam (teamID)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		int groupID = atoi(tokens[1]);
		g->removeTeam(groupID);
	}

	// SPAWN GROUP MANIPULATION
	else if (strcmp(tokens[0], "addspawngroup") == 0) {
		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		SpawnGroup* group = g->addSpawnGroup();
		processCommandResponse("created new SpawnGroup " + to_string(group->getGroupID()), clientID);
	} else if (strcmp(tokens[0], "removespawngroup") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: removespawngroup (groupNum)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		int groupNum = atoi(tokens[1]);
		g->removeSpawnGroup(groupNum);
	} else if (strcmp(tokens[0], "listspawngroups") == 0) {
		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		processCommandResponse("spawn groups:", clientID);
		Iterator<SpawnGroup*> it = g->getSpawnGroups()->getIterator();
		while (it.hasNext()) {
			SpawnGroup* curr = it.next();
			
			processCommandResponse("-- group " + to_string(curr->getGroupID()) + ":", clientID);
			Iterator<Coord*> itCoords = curr->getTiles()->getIterator();
			while (itCoords.hasNext()) {
				Coord* currCoord = itCoords.next();
				processCommandResponse("---- " + to_string(currCoord->x) + "," + to_string(currCoord->y), clientID);
			}
		}
	} else if (strcmp(tokens[0], "addspawngrouptile") == 0) {
		if (numTokens != 4) {
			processCommandResponse("wrong number of arguments: addspawngrouptile (groupNum) (x) (y)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		int groupNum = atoi(tokens[1]);
		Coord pos = { atoi(tokens[2]), atoi(tokens[3]) };
		SpawnGroup* group = g->getSpawnGroupByID(groupNum);

		if (group == NULL) {
			processCommandResponse("spawngroup " + to_string(groupNum) + " does not exist", clientID);
			destroyTokens(tokens);
			return;
		}

		if (group->containsTile(pos)) {
			processCommandResponse("spawngroup " + to_string(groupNum) + " already contains tile (" + to_string(pos.x) + "," + to_string(pos.y) + ")", clientID);
			destroyTokens(tokens);
			return;
		}

		group->addTile(pos);
	} else if (strcmp(tokens[0], "removespawngrouptile") == 0) {
		if (numTokens != 4) {
			processCommandResponse("wrong number of arguments: removespawngrouptile (groupNum) (x) (y)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		int groupNum = atoi(tokens[1]);
		Coord pos = { atoi(tokens[2]), atoi(tokens[3]) };
		SpawnGroup* group = g->getSpawnGroupByID(groupNum);

		if (group == NULL) {
			processCommandResponse("spawngroup " + to_string(groupNum) + " does not exist", clientID);
			destroyTokens(tokens);
			return;
		}

		if (!group->containsTile(pos)) {
			processCommandResponse("spawngroup " + to_string(groupNum) + " doesn't contain tile (" + to_string(pos.x) + "," + to_string(pos.y) + ")", clientID);
			destroyTokens(tokens);
			return;
		}

		group->removeTile(pos);
	}
	
	// LEVEL MANIPULATION
	else if (strcmp(tokens[0], "startgame") == 0) {
		Message m;
		m.type = MSGTYPE_STARTGAME;
		m.clientID = 0;
		_server->recieveMessage(m);
	} else if (strcmp(tokens[0], "save") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: save (filename)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		g->saveLevel(std::string(tokens[1]));
	} else if (strcmp(tokens[0], "settile") == 0) {
		if (numTokens != 4) {
			processCommandResponse("wrong number of arguments: settile (x) (y) (tiletype)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		// TODO: check for invalid values

		int xPos = atoi(tokens[1]);
		int yPos = atoi(tokens[2]);
		TILE tileType = (TILE)atoi(tokens[3]);
		g->setTileAt({ xPos, yPos }, tileType);
	} else if (strcmp(tokens[0], "settilerect") == 0) {
		if (numTokens != 6) {
			processCommandResponse("wrong number of arguments: settilerect (x) (y) (width) (height) (tiletype)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		Coord pos = { atoi(tokens[1]), atoi(tokens[2]) };
		Coord dims = { atoi(tokens[3]), atoi(tokens[4]) };
		TILE type = (TILE)atoi(tokens[5]);
		g->fillRectInBoard(type, pos, dims);
	}
	
	// PRINTING AND DEBUGGING
	else if (strcmp(tokens[0], "printgridprograms") == 0) {
		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		Coord topLeft = { g->getLeftBound(), g->getTopBound() };
		Coord bottomRight = { g->getRightBound(), g->getBottomBound() };
		for (int y = topLeft.y; y < bottomRight.y; y++) {
			std::string row = "";
			for (int x = topLeft.x; x < bottomRight.x; x++) {
				if (g->getTileAt({ x, y }) == TILE_NONE)
					row += " ";
				else {
					if (g->getProgramAt({ x, y }) == NULL)
						row += "-";
					else
						row += to_string(g->getProgramAt({ x, y })->getProgramID());
				}
			}
			processCommandResponse(row, clientID);
		}
	} else if (strcmp(tokens[0], "printgridspawngroups") == 0) {
		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		Coord topLeft = { g->getLeftBound(), g->getTopBound() };
		Coord bottomRight = { g->getRightBound(), g->getBottomBound() };
		for (int y = topLeft.y; y < bottomRight.y; y++) {
			std::string row = "";
			for (int x = topLeft.x; x < bottomRight.x; x++) {
				if (g->getTileAt({ x, y }) == TILE_SPAWN || g->getTileAt({ x, y }) == TILE_SPAWN2) {
					if (g->getSpawnGroupAt({ x, y }) == NULL)
						row += "X";
					else
						row += to_string(g->getSpawnGroupAt({ x, y })->getGroupID());
				} else if (g->getTileAt({ x, y }) == TILE_NONE) {
					row += " ";
				} else {
					row += "-";
				}
			}
			processCommandResponse(row, clientID);
		}
	} 
	
	// MISC
	else if (strcmp(tokens[0], "giveprogram") == 0) {
		if (numTokens != 4) {
			processCommandResponse("wrong number of arguments: giveprogram (user) (programtype) (number of programs)", clientID);
			destroyTokens(tokens);
			return;
		}

		User* u = _server->getUserByName(std::string(tokens[1]));
		if (u == NULL) {
			processCommandResponse("user " + std::string(tokens[1]) + " not found", clientID);
			destroyTokens(tokens);
			return;
		}

		int progType = atoi(tokens[2]);
		if (progType >= PROGRAM_NUM_PROGTYPES || progType < 0) {
			processCommandResponse("program type " + std::string(tokens[2]) + " out of range", clientID);
			destroyTokens(tokens);
			return;
		}

		// increment the number of programs this user owns
		int numProgs = atoi(tokens[3]);
		u->progsOwned_[progType] += numProgs;
		_server->saveUsers();

		// get the client currently controlling this user
		PipeServerside* client = NULL;
		Iterator<PipeServerside*> it = _server->getClientList()->getIterator();
		while (it.hasNext()) {
			PipeServerside* curr = it.next();
			if (curr->getUser() == std::string(tokens[1]))
				client = curr;
		}

		// send all clients a message indicating this client now has this many programs
		if (client != NULL) {
			Message m;
			m.type = MSGTYPE_PROGINVENTORY;
			m.progType = (PROGRAM)progType;
			m.num = u->progsOwned_[progType];
			m.clientID = client->getClientID();
			_server->sendMessageToAllClients(m);
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

	log("SERVER: parser loop existed for some reason\n");
}