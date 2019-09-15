#include "Standard.h"
#include "Server.h"

#include "Game.h"
#include "Message.h"
#include "Pipe.h"
#include "MiscUtil.h"
#include "Program.h"
#include "Player.h"
#include "AICore.h"
#include "Team.h"
#include "User.h"

Server::Server() {
	clients_ = new LinkedList<Pipe*>();
	msgQueue_ = new LinkedList<Message*>();
	users_ = new LinkedList<User*>();
	loadUsers();
	game_ = NULL;
	ownerClient_ = NULL;
}

Server::~Server() {
	//dtor
}

void Server::tick(int ms) {
	mtx.lock();
	while (msgQueue_->getLength() > 0) {
		Message* m = msgQueue_->removeFirst();
		processMessage(m);
		delete m;
	}
	mtx.unlock();
}

void Server::sendMessageToAllClients(Message message) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		if (curr->getUser() != NULL)
			curr->sendData(message);
	}
}

void Server::sendMessageToClient(Message message, int clientID) {
	Pipe* curr = getClientByID(clientID);
	if (curr->getUser() != NULL)
		curr->sendData(message);
}

void Server::sendMessageToAllClientsExcept(Message message, int clientID) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		if (curr->getClientID() != clientID && curr->getUser() != NULL)
			curr->sendData(message);
	}
}

Pipe* Server::getClientByID(int clientID) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		if (curr->getClientID() == clientID) {
			return curr;
		}
	}

	// if a client with a matching ID wasn't found, return null
	return NULL;
}

void Server::recieveMessage(Message message) {
	mtx.lock();
	msgQueue_->addLast(new Message(message));
	mtx.unlock();
}

void Server::login(Pipe* client, User* user) {
	// set this client's user
	client->setUser(user);
	client->setNewUser(NULL);

	// if there is no owner client, make this it
	if (ownerClient_ == NULL)
		ownerClient_ = client;

	// send message to all clients except this one
	Message m;
	m.clientID = client->getClientID();
	m.actionID = (client == ownerClient_) ? 9000 : 0;
	m.type = MSGTYPE_LOGIN;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, client->getUser()->username_.c_str(), DEFAULT_MSG_TEXTSIZE);
	sendMessageToAllClientsExcept(m, client->getClientID());

	// send the issuing client a message for every client currently connected
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();

		// check that this client is logged in
		if (curr->getUser() == NULL)
			continue;

		// send the newly connected client a list of all clients
		Message m;
		m.type = MSGTYPE_LOGIN;
		m.clientID = curr->getClientID();
		strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, curr->getUser()->username_.c_str(), DEFAULT_MSG_TEXTSIZE);
		m.actionID = (curr == ownerClient_) ? 9000 : 0;
		client->sendData(m);
	}

	// send the issuing client the current gamemode
	m.type = MSGTYPE_GAMECONFIG;
	if (gameMode_ == GAMEMODE_COOP)
		m.gameConfigType = MSGGAMECONFIGTYPE_COOP;
	else if (gameMode_ == GAMEMODE_FFA)
		m.gameConfigType = MSGGAMECONFIGTYPE_FFA;
	else if (gameMode_ == GAMEMODE_TEAMDM)
		m.gameConfigType = MSGGAMECONFIGTYPE_TEAMDM;
	client->sendData(m);
}

void Server::processMessage(Message* msg) {
	printMessage(*msg);

	// get the client of the issuer
	Pipe* issuingClient = getClientByID(msg->clientID);
	if (issuingClient == NULL && msg->clientID != 0) {
		printf("SERVER ERR: got a message from a client that doesn't exist\n");
		return;
	}

	// if this client isn't logged in (and isn't the server)
	if (issuingClient != NULL && issuingClient->getUser() == NULL) {
		if (msg->type == MSGTYPE_CREATEUSER) {
			User* newUser = issuingClient->getNewUser();

			if (msg->infoType == MSGINFOTYPE_USERNAME) {
				if (std::string(msg->text).length() <= 0) {
					printf("SERVER ERR: tried to create an account with a blank username\n");

					Message m;
					m.type = MSGTYPE_CREATEUSER;
					strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: username cannot be blank", DEFAULT_MSG_TEXTSIZE);
					issuingClient->sendData(m);
				} else if (getUserByName(std::string(msg->text)) != NULL) {
					printf("SERVER ERR: tried to create an account with a name that already exists\n");

					Message m;
					m.type = MSGTYPE_CREATEUSER;
					strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: username already taken", DEFAULT_MSG_TEXTSIZE);
					issuingClient->sendData(m);
				} else {
					if (newUser != NULL)
						delete newUser;
					newUser = new User();
					newUser->username_ = std::string(msg->text);
					issuingClient->setNewUser(newUser);
				}

			} else if (msg->infoType == MSGINFOTYPE_PASSWORD) {
				if (newUser == NULL) {
					printf("SERVER ERR: received a new User's password before receiving its username\n");
				} else if (std::string(msg->text).length() <= 0) {
					printf("SERVER ERR: tried to create an account with a blank password\n");

					Message m;
					m.type = MSGTYPE_CREATEUSER;
					strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: password cannot be blank", DEFAULT_MSG_TEXTSIZE);
					issuingClient->sendData(m);
				} else {
					// create the new user
					newUser->password_ = std::string(msg->text);
					users_->addFirst(newUser);
					saveUsers();

					// log in the user
					login(issuingClient, newUser);
				}
			}
		} else if (msg->type == MSGTYPE_LOGIN) {
			User* tempUser = issuingClient->getTempUser();

			if (msg->infoType == MSGINFOTYPE_USERNAME) {
				if (std::string(msg->text).length() <= 0) {
					printf("SERVER ERR: tried to log into an account with a blank username\n");

					Message m;
					m.type = MSGTYPE_CREATEUSER;
					strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: username cannot be blank", DEFAULT_MSG_TEXTSIZE);
					issuingClient->sendData(m);
				} else {
					tempUser = getUserByName(std::string(msg->text));
					if (tempUser == NULL) {
						printf("SERVER ERR: requested user does not exist in database\n");

						Message m;
						m.type = MSGTYPE_CREATEUSER;
						strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: user not found", DEFAULT_MSG_TEXTSIZE);
						issuingClient->sendData(m);
					} else if (tempUser->loggedIn_) {
						printf("SERVER ERR: requested user is already logged in\n");

						Message m;
						m.type = MSGTYPE_CREATEUSER;
						strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: user already logged in", DEFAULT_MSG_TEXTSIZE);
						issuingClient->sendData(m);
					} else {
						tempUser->loggedIn_ = true;
						issuingClient->setTempUser(tempUser);
					}
				}
			} else if (msg->infoType == MSGINFOTYPE_PASSWORD) {
				if (tempUser == NULL)
					printf("SERVER ERR: attempted to pass a user's password before passing a valid username\n");
				else if (std::string(msg->text).length() <= 0) {
					printf("SERVER ERR: tried to log into an account with a blank password\n");

					Message m;
					m.type = MSGTYPE_CREATEUSER;
					strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: password cannot be blank", DEFAULT_MSG_TEXTSIZE);
					issuingClient->sendData(m);
				} else {
					if (tempUser->password_.compare(std::string(msg->text)) == 0) {
						// log in this client
						login(issuingClient, tempUser);
					} else {
						printf("SERVER: client tried to log into user %s with wrong password %s\n", tempUser->username_.c_str(), msg->text);
						issuingClient->setTempUser(NULL);

						Message m;
						m.type = MSGTYPE_CREATEUSER;
						strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: incorrect password", DEFAULT_MSG_TEXTSIZE);
						issuingClient->sendData(m);
					}
				}
			}
		}
		return;
	}

	// process the incoming message and then send it to all clients
	// assuming the client is logged in or is the server
	switch (msg->type) {
	case MSGTYPE_TEXT:
		// check if usable as a command
		if ((msg->text[0] == '/' && issuingClient == ownerClient_) || issuingClient == NULL) {
			std::thread doCommand(&Server::processCommand, this, std::string(&msg->text[1]));
			doCommand.detach();
		} else
			// forward to all clients
			sendMessageToAllClients(*msg);
		break;
	case MSGTYPE_MOVE:
		if (game_->getStatus() != GAMESTATUS_PLAYING)
			return;

		if (game_->getCurrTurnPlayer() != issuingClient->getPlayer())
			return;

		{
			Player* p = issuingClient->getPlayer();
			p->moveSelectedProgram(msg->pos);
		}
		break;
	case MSGTYPE_CONNECT:
		// put the clientID back
		msg->clientID = msg->actionID;

		// if there is no owner client, make this it
		if (ownerClient_ == NULL)
			ownerClient_ = getClientByID(msg->clientID);

		// set this client's name
		//getClientByID(msg->clientID)->setName(msg->text);

		// send a connect message for the new client to all *other* clients
		sendMessageToAllClientsExcept(*msg, msg->clientID);

		// send the issuing client a message for every client currently connected
		{
			Iterator<Pipe*> it = clients_->getIterator();
			while (it.hasNext()) {
				Pipe* curr = it.next();

				// send the newly connected client a list of all clients
				Message m;
				m.type = MSGTYPE_CONNECT;
				m.clientID = curr->getClientID();
				//strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, curr->getName().c_str(), DEFAULT_MSG_TEXTSIZE);
				if (curr == ownerClient_)
					m.actionID = 9000; // an arbitrary value to indicate this is the owner client
				else
					m.actionID = 0;
				sendMessageToClient(m, msg->clientID);
			}
		}
		break;
	case MSGTYPE_LOAD:
		if (issuingClient == ownerClient_ || issuingClient == NULL) {
			if (game_ != NULL)
				delete game_;

			game_ = new Game("levels/classic/" + to_string(msg->levelNum) + ".urf");
			sendMessageToAllClients(*msg);
		} else {
			Message err;
			err.type = MSGTYPE_ERROR;
			strncpy_s(err.text, DEFAULT_MSG_TEXTSIZE, "ERR: cannot load a level if not the server owner", DEFAULT_MSG_TEXTSIZE);
			issuingClient->sendData(err);
		}
		break;
	case MSGTYPE_RESYNCGAME:
		resyncClient(msg->clientID);
		break;
	case MSGTYPE_JOIN:
		if (game_ == NULL) {
			printf("SERVER ERR: client %i tried to join a game that doesn't exist\n", msg->clientID);
			return;
		} else if (issuingClient->getPlayer() == NULL) {
			printf("SERVER: client %i has joined the current game\n", msg->clientID);

			// if team 0 doesn't exist, create it
			if (game_->getTeamByNum(0) == NULL)
				game_->getAllTeams()->addFirst(new Team(0));

			// create a new player in the local game, assign it a unique playerID, assign it by default to team 0
			int team = 0;
			Player* newPlayer = new Player(game_, team);
			msg->team = 0;
			msg->playerID = issuingClient->getClientID();
			newPlayer->setPlayerID(msg->playerID);
			Team* teamPtr = game_->getTeamByNum(team);
			teamPtr->getAllPlayers()->addFirst(newPlayer);
			issuingClient->setPlayer(newPlayer);

			// send this client's new unique playerID to every client
			sendMessageToAllClients(*msg);
		} else {
			printf("SERVER ERR: client %i trying to join an already-joined game\n", msg->clientID);
			return;
		}
		break;
	case MSGTYPE_SELECT:
	{
		if (msg->selectType == MSGSELECTTYPE_TILE) {
			Player* p = issuingClient->getPlayer();
			p->setSelectedTile(msg->pos);
		} else if (msg->selectType == MSGSELECTTYPE_ACTION) {
			Player* p = issuingClient->getPlayer();
			p->setSelectedAction(p->getSelectedProgram()->getActions()->getObjectAt(msg->actionID));
		}
		break;
	}
	case MSGTYPE_LEAVE:
	{
		// this case is triggered ONLY if the client voluntarily and intentionally disconnects
		// do NOT do anything else in this case
		disconnect(issuingClient);
		break;
	}
	case MSGTYPE_INFO:
		if (msg->infoType == MSGINFOTYPE_GAMESTATUS) {
			if (issuingClient == ownerClient_)
				game_->setStatus(msg->statusType);
			else {
				Message err;
				err.type = MSGTYPE_ERROR;
				strncpy_s(err.text, DEFAULT_MSG_TEXTSIZE, "ERR: only the owner can change the game state", DEFAULT_MSG_TEXTSIZE);
				issuingClient->sendData(err);
			}
		}
		break;
	case MSGTYPE_NEXTTURN:
	{
		// get the player that just ended its turn
		Player* p = game_->getPlayerByID(msg->playerID);

		// check if this message actually came from the current player
		if (p->getPlayerID() != game_->getCurrTurnPlayer()->getPlayerID()) {
			printf("SERVER ERR: NEXTTURN message received from player %i, but current player is %i\n", p->getPlayerID(), game_->getCurrTurnPlayer()->getPlayerID());
			return;
		}

		// end this player's turn
		p->endTurn();

		// get and set the next player
		Player* nextPlayer = game_->getFollowingPlayer(game_->getCurrTurnPlayer());
		game_->setCurrTurnPlayer(nextPlayer);

		// if the next player has a mind, process its turns
		if (nextPlayer->getMind() != NULL) {
			std::thread aiTurn(&Server::processAITurn, this, nextPlayer);
			aiTurn.detach();
		}
	}
	break;
	case MSGTYPE_GAMECONFIG:
		if (issuingClient != ownerClient_) {
			Message m;
			m.type = MSGTYPE_ERROR;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERROR: only the owner can configure the game", DEFAULT_MSG_TEXTSIZE);
			issuingClient->sendData(m);
			break;
		}

		if (msg->gameConfigType == MSGGAMECONFIGTYPE_COOP)
			gameMode_ = GAMEMODE_COOP;
		else if (msg->gameConfigType == MSGGAMECONFIGTYPE_FFA)
			gameMode_ = GAMEMODE_FFA;
		else if (msg->gameConfigType == MSGGAMECONFIGTYPE_TEAMDM)
			gameMode_ = GAMEMODE_TEAMDM;

		sendMessageToAllClients(*msg);
		break;
	case MSGTYPE_ACTION:
		issuingClient->getPlayer()->useSelectedActionAt(msg->pos);
		break;
	case MSGTYPE_PLACEPROG:
	{
		// check for correct game status
		if (game_->getStatus() != GAMESTATUS_PLACING_PROGRAMS) {
			Message err;
			err.type = MSGTYPE_ERROR;
			strncpy_s(err.text, DEFAULT_MSG_TEXTSIZE, "ERR: what did you do? the gamestatus does not indicate placing programs", DEFAULT_MSG_TEXTSIZE);
			return;
		}

		// check for spawn tile
		if (game_->getTileAt(msg->pos) != TILE_SPAWN && game_->getTileAt(msg->pos) != TILE_SPAWN2) {
			Message err;
			err.type = MSGTYPE_ERROR;
			strncpy_s(err.text, DEFAULT_MSG_TEXTSIZE, "ERR: you can only place a program on a spawn tile", DEFAULT_MSG_TEXTSIZE);
			return;
		}

		// remove whatever program is currently there, if any, and give it back to its user
		Program* currProg = game_->getProgramAt(msg->pos);
		if (currProg != NULL) {
			User* u = NULL;
			Iterator<Pipe*> it = clients_->getIterator();
			while (it.hasNext()) {
				Pipe* curr = it.next();
				if (curr->getPlayer() == currProg->getOwner())
					u = curr->getUser();
			}

			if (u == NULL)
				printf("SERVER ERR?: replaced program had no related user?\n");

			u->progs_[currProg->getType()]++;
			currProg->getOwner()->getProgList()->remove(currProg);
			game_->setProgramAt(msg->pos, NULL);
			delete currProg;
		}

		// place the new program, send a message indicating this to all clients
		Message response;
		response.type = MSGTYPE_PLACEPROG;
		response.team = 0;
		if (msg->progType != PROGRAM_NONE) {
			Program* p = new Program(msg->progType, 0, msg->pos);
			issuingClient->getUser()->progs_[msg->progType]--;
			p->setProgramID(randInt());
			game_->getPlayerByID(msg->playerID)->addProgram(p);
			game_->setProgramAt(msg->pos, p);

			response.programID = p->getProgramID();
		}
		response.pos = msg->pos;
		response.progType = msg->progType;
		response.playerID = msg->playerID;
		response.clientID = msg->clientID;

		sendMessageToAllClients(response);
	}
	break;
	}
}

void Server::connect(SOCKET client) {
	Pipe* newPipe = new Pipe(client, this);
	clients_->addFirst(newPipe);

	std::thread newThread(&Pipe::listenData, newPipe);
	newThread.detach();
}

void Server::disconnect(Pipe* client) {
	if (!client->isClosed()) {
		// kill the thread formally so that death can begin; the pipe will call this method again
		client->close();
		return;
	}

	// let the server know
	printf("Client %i disconnected\n", client->getClientID());

	// remove client from list
	clients_->remove(client);

	// if this client was the owner
	if (client == ownerClient_) {
		if (clients_->getLength() == 0)
			ownerClient_ = NULL;
		else
			ownerClient_ = clients_->getFirst();
	}

	// remove the player from the game
	if (game_ != NULL) {
		Player* p = client->getPlayer();
		Team* t = game_->getTeamByNum(p->getTeam());
		t->getAllPlayers()->remove(p);
		delete p;
	}

	// send leave message to all connected clients
	Message msg;
	msg.type = MSGTYPE_LEAVE;
	msg.clientID = client->getClientID();
	sendMessageToAllClients(msg);
}

void Server::quitAll() {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();

		if (curr->getPlayer() != NULL) {
			curr->setPlayer(NULL);

			Message m;
			m.type = MSGTYPE_LEAVE;
			m.clientID = curr->getClientID();
			sendMessageToAllClients(m);
		}
	}
}

void Server::resyncAll() {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		resyncClient(it.next()->getClientID());
	}
}

void Server::resyncClient(int clientID) {
	// initialize message
	Message msg;
	msg.clientID = 0;

	// send background info
	msg.type = MSGTYPE_INFO;
	msg.infoType = MSGINFOTYPE_BKG;
	msg.bkgType = game_->getBackground();
	sendMessageToClient(msg, clientID);

	// send tiles and items
	for (int x = game_->getLeftBound(); x < game_->getRightBound(); x++)
		for (int y = game_->getTopBound(); y < game_->getBottomBound(); y++) {
			msg.infoType = MSGINFOTYPE_TILE;
			msg.pos = Coord{ x, y };
			msg.tileType = game_->getTileAt(msg.pos);
			sendMessageToClient(msg, clientID);

			if (game_->getItemAt(msg.pos) != ITEM_NONE) {
				msg.infoType = MSGINFOTYPE_ITEM;
				msg.itemType = game_->getItemAt(msg.pos);
				sendMessageToClient(msg, clientID);
			}
		}

	// send players and programs
	LinkedList<Team*>* allTeams = game_->getAllTeams();
	for (int teamNum = 0; teamNum < allTeams->getLength(); teamNum++) {
		LinkedList<Player*>* currTeam = allTeams->getObjectAt(teamNum)->getAllPlayers();
		for (int playerNum = 0; playerNum < currTeam->getLength(); playerNum++) {
			Player* currPlayer = currTeam->getObjectAt(playerNum);
			LinkedList<Program*>* progList = currPlayer->getProgList();
			for (int progNum = 0; progNum < progList->getLength(); progNum++) {
				Program* currProg = progList->getObjectAt(progNum);
				LinkedList<Coord*>* progCoords = currProg->getTiles();
				for (int coordNum = 0; coordNum < progCoords->getLength(); coordNum++) {
					msg.infoType = MSGINFOTYPE_PROGRAM;
					msg.playerID = currPlayer->getPlayerID();
					msg.programID = currProg->getProgramID();
					msg.progType = currProg->getType();
					msg.pos = *(progCoords->getObjectAt(coordNum));
					msg.team = teamNum;
					sendMessageToClient(msg, clientID);
				}
			}
		}
	}

	// send the current game status
	msg.type = MSGTYPE_INFO;
	msg.infoType = MSGINFOTYPE_GAMESTATUS;
	msg.statusType = game_->getStatus();
	sendMessageToClient(msg, clientID);
}

LinkedList<Pipe*>* Server::getClientList() {
	return clients_;
}

void Server::processAITurn(Player* p) {
	// execute this player's moves and actions
	AICore* core = p->getMind();
	if (core == NULL) {
		printf("SERVER ERR: AI player %i doesn't have a mind, although it should\n", p->getPlayerID());
	} else
		while (!p->getDoneTurn()) {
			core->act(1);
			Sleep(200);
		}

	// send a message back to the server indicating that this players' turn is done
	Message m;
	m.type = MSGTYPE_NEXTTURN;
	m.clientID = 0;
	m.playerID = p->getPlayerID();
	recieveMessage(m);
}

void Server::processCommand(std::string cmd) {
	// tokenize
	// (gotta add a newline because that's how the tokenizer works)
	char* tokens[1024];
	int numTokens = tokenize(tokens, (cmd + "\n").c_str(), ' ');

	// process tokens
	if (numTokens == 0) {
		printf(">> no command entered\n");
	} else if (strcmp(tokens[0], "getowner") == 0) {
		if (ownerClient_ == NULL)
			printf(">> there is no owner client\n");
		else {
			printf(">> the owner client is %i", ownerClient_->getClientID());
			if (ownerClient_->getUser() != NULL)
				printf(" <logged in as %s>\n", ownerClient_->getUser()->username_.c_str());
			else
				printf("\n");
		}
	} else if (strcmp(tokens[0], "listclients") == 0) {
		printf(">> connected clientIDs:\n");
		Iterator<Pipe*> it = clients_->getIterator();
		while (it.hasNext()) {
			Pipe* curr = it.next();
			printf(">> %i ", curr->getClientID());
			if (curr->getUser() != NULL)
				printf("<logged in as %s>\n", curr->getUser()->username_.c_str());
			else
				printf("<not logged in>\n");
		}
	} else if (strcmp(tokens[0], "kick") == 0) {
		if (numTokens != 2) {
			printf(">> wrong number of arguments:\nkick (clientID)\n");
			printf(">> kick (clientID)\n");
			destroyTokens(tokens);
			return;
		}

		int clientID = atoi(tokens[1]);
		if (clientID == 0) {
			printf(">> cannot kick clientID 0: is server\n");
			destroyTokens(tokens);
			return;
		}
		printf(">> attempting to kick clientID %i\n", clientID);

		Message m;
		m.type = MSGTYPE_LEAVE;
		m.clientID = clientID;
		recieveMessage(m);
	} else if (strcmp(tokens[0], "load") == 0) {
		if (numTokens != 2) {
			printf(">> wrong number of arguments:\nload (levelnum)\n");
			printf(">> load (levelnum)\n");
			destroyTokens(tokens);
			return;
		}

		int levelNum = atoi(tokens[1]);

		Message m;
		m.type = MSGTYPE_LOAD;
		m.levelNum = levelNum;
		m.clientID = 0;
		recieveMessage(m);
	} else if (strcmp(tokens[0], "help") == 0) {
		printf(">> Available commands:\n");
		printf(">> listclients\n");
		printf(">> kick (clientID)\n");
		printf(">> load (levelnum)\n");
		printf(">> say (string)\n");
		printf(">> listplayers\n");
		printf(">> adduser (username) (password)\n");
		printf(">> deluser (username)\n");
		printf(">> listusers\n");
		printf(">> getowner\n");
	} else if (strcmp(tokens[0], "say") == 0) {
		std::string ret = "";
		for (int i = 1; i < numTokens; i++)
			ret += std::string(tokens[i]) + " ";

		Message m;
		m.type = MSGTYPE_TEXT;
		m.clientID = 0;
		strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, ret.c_str(), DEFAULT_MSG_TEXTSIZE);
		recieveMessage(m);
	} else if (strcmp(tokens[0], "listplayers") == 0) {
		if (game_ == NULL) {
			printf(">> cannot list players - no game currently in session\n");
			destroyTokens(tokens);
			return;
		}

		Iterator<Team*> itTeam = game_->getAllTeams()->getIterator();
		while (itTeam.hasNext()) {
			Team* currTeam = itTeam.next();
			printf(">> Team %i\n", currTeam->getTeamNum());

			Iterator<Player*> itPlayer = currTeam->getAllPlayers()->getIterator();
			while (itPlayer.hasNext()) {
				Player* pCurr = itPlayer.next();
				if (pCurr->getMind() == NULL)
					printf(">> Player %i - Human\n", pCurr->getPlayerID());
				else
					printf(">> Player %i - AI\n", pCurr->getPlayerID());
			}
		}
	} else if (strcmp(tokens[0], "adduser") == 0) {
		if (numTokens != 3) {
			printf(">> wrong number of arguments:\n");
			printf(">> adduser (username) (password)\n");
			destroyTokens(tokens);
			return;
		}

		User* u = new User();
		u->username_ = std::string(tokens[1]);
		u->password_ = std::string(tokens[2]);

		if (getUserByName(u->username_) == NULL) {
			users_->addFirst(u);
			saveUsers();
			printf(">> successfully created user %s with password %s\n", tokens[1], tokens[2]);
		} else {
			delete u;
			printf(">> user creation failed\n");
			printf(">> a user with name \"%s\" already exists\n", u->username_.c_str());
		}
	} else if (strcmp(tokens[0], "deluser") == 0) {
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
			users_->remove(u);
			delete u;
			saveUsers();
			printf(">> successfully deleted user %s\n", tokens[1]);
		}
	} else if (strcmp(tokens[0], "listusers") == 0) {
		printf(">> Users:\n");
		users_->forEach([] (User* u) {printf(">> %s : %s\n", u->username_.c_str(), u->password_.c_str()); });
	} else {
		printf(">> \"%s\" is not a valid command\n", cmd.c_str());
		printf(">> use \"help\" for a list of commands\n");
	}

	// destroy tokens
	destroyTokens(tokens);
}

void Server::processCommandLoop() {
	while (true) {
		// get the line
		std::string buffer;
		getline(std::cin, buffer);

		// process it
		processCommand(buffer);
	}

	printf("parser loop existed for some reason\n");
}

void Server::loadUsers() {
	std::ifstream userStream;
	userStream.open("users.dat", std::ios::in | std::ios::binary);

	if (!userStream.is_open()) {
		printf("SERVER ERROR: could not open users.dat\n");
	} else {
		printf("SERVER: reading users.dat\n");

		// read the sizes of various data types
		printf("SERVER: reading constants\n");
		int8_t sizeOfInt;
		userStream.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		userStream.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		userStream.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		userStream.read((char*)&sizeOfBool, 1);
		printf("SERVER: read constants: int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);

		// read user table
		int numUsers;
		userStream.read((char*)&numUsers, sizeOfInt);
		printf("SERVER: reading %i users\n", numUsers);
		for (int i = 0; i < numUsers; i++) {
			User* currUser = new User();

			char usernameRaw[DEFAULT_MSG_TEXTSIZE];
			userStream.read(usernameRaw, sizeOfChar*DEFAULT_MSG_TEXTSIZE);
			currUser->username_ = std::string(usernameRaw);

			char passwordRaw[DEFAULT_MSG_TEXTSIZE];
			userStream.read(passwordRaw, sizeOfChar*DEFAULT_MSG_TEXTSIZE);
			currUser->password_ = std::string(passwordRaw);

			userStream.read((char*)currUser->progs_, sizeOfInt*PROGRAM_NUM_PROGTYPES);

			userStream.read((char*)&currUser->numWins_, sizeOfInt);
			userStream.read((char*)&currUser->numLosses_, sizeOfInt);
			userStream.read((char*)&currUser->numDraws_, sizeOfInt);
			userStream.read((char*)&currUser->numResigns_, sizeOfInt);
			userStream.read((char*)&currUser->numGames_, sizeOfInt);

			users_->addFirst(currUser);
		}

		// close the file
		userStream.close();
		printf("SERVER: successfully read user table\n");
	}
}

void Server::saveUsers() {
	std::ofstream userStream;
	userStream.open("users.dat", std::ios::out | std::ios::binary | std::ios::trunc);

	if (!userStream.is_open()) {
		printf("SERVER ERROR: could not open users.dat\n");
	} else {
		printf("SERVER: writing users.dat\n");

		// begin by writing the sizes of various data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		printf("SERVER: saving constants... int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);
		userStream.write((char*)&sizeOfInt, 1);
		userStream.write((char*)&sizeOfChar, 1);
		userStream.write((char*)&sizeOfDouble, 1);
		userStream.write((char*)&sizeOfBool, 1);

		// write user table
		int numUsers = users_->getLength();
		userStream.write((char*)&numUsers, sizeOfInt);
		printf("SERVER: writing %i users\n", numUsers);
		for (int i = 0; i < numUsers; i++) {
			User* currUser = users_->getObjectAt(i);
			userStream.write(currUser->username_.c_str(), sizeOfChar*DEFAULT_MSG_TEXTSIZE);
			userStream.write(currUser->password_.c_str(), sizeOfChar*DEFAULT_MSG_TEXTSIZE);

			userStream.write((char*)currUser->progs_, sizeOfInt*PROGRAM_NUM_PROGTYPES);

			userStream.write((char*)&currUser->numWins_, sizeOfInt);
			userStream.write((char*)&currUser->numLosses_, sizeOfInt);
			userStream.write((char*)&currUser->numDraws_, sizeOfInt);
			userStream.write((char*)&currUser->numResigns_, sizeOfInt);
			userStream.write((char*)&currUser->numGames_, sizeOfInt);
		}

		// close the file
		userStream.close();
		printf("SERVER: successfully wrote user table\n");
	}
}

User* Server::getUserByName(std::string name) {
	Iterator<User*> it = users_->getIterator();
	while (it.hasNext()) {
		User* curr = it.next();
		if (curr->username_.compare(name) == 0)
			return curr;
	}

	return NULL;
}