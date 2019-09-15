#include "Standard.h"
#include "Server.h"

#include <iostream>
#include <string>

#include "Game.h"
#include "Message.h"
#include "MiscUtil.h"
#include "Program.h"
#include "Player.h"
#include "AICore.h"
#include "Team.h"
#include "User.h"
#include "Pipe.h"
#include "CommandProcessor.h"

Server::Server(bool isLocal, std::string savePath) {
	game_ = NULL;
	ownerClient_ = NULL;

	isLocal_ = isLocal;

	savePath_ = savePath;
	currentLevel_ = 0;

	clients_ = new LinkedList<Pipe*>();
	msgQueue_ = new LinkedList<Message*>();
	users_ = new LinkedList<User*>();
	loadUsers();
}

Server::~Server() {
	if (game_ != NULL)
		delete game_;

	while (clients_->getLength() > 0)
		delete clients_->poll();
	delete clients_;
	while (msgQueue_->getLength() > 0)
		delete msgQueue_->poll();
	delete msgQueue_;
	while (users_->getLength() > 0)
		delete users_->poll();
	delete users_;
}

void Server::processAllMessages() {
	mtx_.lock();
	while (msgQueue_->getLength() > 0) {
		Message* m = msgQueue_->removeFirst();
		processMessage(m);
		delete m;
	}
	mtx_.unlock();
}

void Server::sendMessageToAllClients(Message message) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		if (curr->getUser() != "") // dont send messages to clients that aren't logged in
			curr->sendData(message);
	}
}

void Server::sendMessageToClient(Message message, int clientID) {
	Pipe* curr = getClientByID(clientID);
	if (curr->getUser() != "")
		curr->sendData(message);
}

void Server::sendMessageToAllClientsExcept(Message message, int clientID) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		if (curr->getClientID() != clientID && curr->getUser() != "")
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
	mtx_.lock();
	msgQueue_->addLast(new Message(message));
	mtx_.unlock();
}

// attempts to log a Client in
void Server::tryLogin(Pipe* client, Message m) {
	char* tokens[1024];
	int numTokens = tokenize(tokens, (std::string(m.text) + "\n").c_str(), '\n');
	std::string username = std::string(tokens[0]);
	std::string password = std::string(tokens[1]);

	if (m.type == MSGTYPE_CREATEUSER) {
		if (username.length() <= 0) {
			log("SERVER ERR: client tried to create an account with a blank username\n");

			Message m;
			m.type = MSGTYPE_CREATEUSER;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: username cannot be blank", DEFAULT_MSG_TEXTSIZE);
			client->sendData(m);
		} else if (getUserByName(username) != NULL) {
			log("SERVER ERR: client tried to create an account with a name that already exists\n");

			Message m;
			m.type = MSGTYPE_CREATEUSER;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: username already taken", DEFAULT_MSG_TEXTSIZE);
			client->sendData(m);
		} else if (password.length() <= 0) {
			log("SERVER ERR: tried to create an account with a blank password\n");

			Message m;
			m.type = MSGTYPE_CREATEUSER;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: password cannot be blank", DEFAULT_MSG_TEXTSIZE);
			client->sendData(m);
		} else {
			// create and login the new user
			User* newUser = new User();
			newUser->username_ = username;
			newUser->password_ = password;
			users_->addFirst(newUser);
			saveUsers();

			// log in the user
			login(client, newUser);
		}
	} else if (m.type == MSGTYPE_LOGIN) {
		if (username.length() <= 0) {
			log("SERVER ERR: client tried to log into an account with a blank username\n");

			Message m;
			m.type = MSGTYPE_CREATEUSER;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: username cannot be blank", DEFAULT_MSG_TEXTSIZE);
			client->sendData(m);
		} else {
			User* requestedUser = getUserByName(username);
			if (requestedUser == NULL) {
				log("SERVER ERR: requested user does not exist in database\n");

				Message m;
				m.type = MSGTYPE_CREATEUSER;
				strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: user not found", DEFAULT_MSG_TEXTSIZE);
				client->sendData(m);
			} else if (requestedUser->loggedIn_) {
				log("SERVER ERR: requested user is already logged in\n");

				Message m;
				m.type = MSGTYPE_CREATEUSER;
				strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: user already logged in", DEFAULT_MSG_TEXTSIZE);
				client->sendData(m);
			} else if (password.length() <= 0) {
				log("SERVER ERR: tried to log into an account with a blank password\n");

				Message m;
				m.type = MSGTYPE_CREATEUSER;
				strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: password cannot be blank", DEFAULT_MSG_TEXTSIZE);
				client->sendData(m);
			} else {
				if (requestedUser->password_.compare(password) == 0) {
					// log in this client
					login(client, requestedUser);
				} else {
					log("SERVER: client tried to log into user " + requestedUser->username_ + " with wrong password " + password + "\n");

					Message m;
					m.type = MSGTYPE_CREATEUSER;
					strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, "ERR: incorrect password", DEFAULT_MSG_TEXTSIZE);
					client->sendData(m);
				}
			}
		}
	}

	destroyTokens(tokens);
}

// logs a Client in as the given User
void Server::login(Pipe* client, User* user) {
	// set this client's user
	client->setUser(user->username_);

	// if there is no owner client, make this it
	if (ownerClient_ == NULL)
		ownerClient_ = client;

	// send message to all clients except this one
	Message m;
	m.clientID = client->getClientID();
	m.actionID = (client == ownerClient_) ? 9000 : 0;
	m.type = MSGTYPE_LOGIN;
	strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, client->getUser().c_str(), DEFAULT_MSG_TEXTSIZE);
	sendMessageToAllClientsExcept(m, client->getClientID());

	// send the issuing client a message for every client currently connected
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();

		// check that this client is logged in
		if (curr->getUser() == "")
			continue;

		// send the newly connected client a list of all clients
		Message m;
		m.type = MSGTYPE_LOGIN;
		m.clientID = curr->getClientID();
		strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, curr->getUser().c_str(), DEFAULT_MSG_TEXTSIZE);
		m.actionID = ((curr == ownerClient_) ? 9000 : 0);
		client->sendData(m);
	}

	// send the issuing client its credits
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_CREDITS;
	m.num = getUserByName(client->getUser())->numCredits_;
	client->sendData(m);

	// send the issuing client its program inventory contents
	for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
		m.type = MSGTYPE_PROGINVENTORY;
		m.progType = (PROGRAM)i;
		m.num = getUserByName(client->getUser())->progsOwned_[i];
		client->sendData(m);
	}

	// send the issuing client its progress
	if (savePath_ == "levels/classic") {
		for (int i = 0; i < NUM_LEVELS_CLASSIC; i++) {
			if (user->campaignClassic_[i]) {
				m.type = MSGTYPE_LEVELUNLOCK;
				m.num = i;
				client->sendData(m);
			}
		}
	} else if (savePath_ == "levels/nightfall") {
		for (int i = 0; i < NUM_LEVELS_NIGHTFALL; i++) {
			if (user->campaignNightfall_[i]) {
				m.type = MSGTYPE_LEVELUNLOCK;
				m.num = i;
				client->sendData(m);
			}
		}
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
	log("SERVER RECIEVED MSG: ");
	_printMessage(*msg);

	// get the client of the issuer
	Pipe* issuingClient = getClientByID(msg->clientID);
	if (issuingClient == NULL && msg->clientID != 0) {
		log("SERVER ERR: got a message from a client that doesn't exist\n");
		return;
	}

	// if this client isn't logged in (and isn't the server)
	if (issuingClient != NULL && issuingClient->getUser() == "") {
		tryLogin(issuingClient, *msg);
		return;
	}

	// process the incoming message and then send it to all clients
	// assuming the client is logged in or is the server
	switch (msg->type) {
	case MSGTYPE_CHAT:
		// check if usable as a command
		if ((msg->text[0] == '/' && issuingClient == ownerClient_) || issuingClient == NULL) {
			std::thread doCommand(processCommand, std::string(&msg->text[1]), msg->clientID);
			doCommand.detach();
		} else
			// forward to all clients
			sendMessageToAllClients(*msg);
		break;
	case MSGTYPE_MOVE:
		if (game_->getStatus() != GAMESTATUS_PLAYING)
			return;

		if (game_->getCurrTurnPlayer()->getPlayerID() != issuingClient->getPlayer())
			return;

		{
			Player* p = game_->getPlayerByID(issuingClient->getPlayer());
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
		// getClientByID(msg->clientID)->setName(msg->text);

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

			currentLevel_ = msg->num;
			sendMessageToAllClients(*msg);
			game_ = new Game(savePath_ + "/" + to_string(msg->num) + ".urf");

			Iterator<Pipe*> it = clients_->getIterator();
			while (it.hasNext()) {
				it.next()->setPlayer(-1);
			}
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
			log("SERVER ERR: client " + to_string(msg->clientID) + " tried to join a game that doesn't exist\n");
			return;
		} else if (issuingClient->getPlayer() == -1) {
			log("SERVER: client " + to_string(msg->clientID) + " has joined the current game\n");

			// get the default human team, creating one if it doesn't exist
			Team* humanTeam = game_->getDefaultTeamHuman();
			if (humanTeam == NULL) {
				humanTeam = game_->addTeam();
				humanTeam->setDefaultHuman(true);
			}

			// create the new player
			Player* p = game_->addPlayer(humanTeam->getTeamID());
			issuingClient->setPlayer(p->getPlayerID());

			Message m;
			m.type = MSGTYPE_JOIN;
			m.clientID = msg->clientID;
			m.teamID = humanTeam->getTeamID();
			m.playerID = p->getPlayerID();
			sendMessageToAllClients(m);
		} else {
			log("SERVER ERR: client " + to_string(msg->clientID) + " trying to join an already-joined game\n");
			return;
		}
		break;
	case MSGTYPE_SELECT:
	{
		if (msg->selectType == MSGSELECTTYPE_TILE) {
			Player* p = game_->getPlayerByID(issuingClient->getPlayer());
			p->setSelectedTile(msg->pos);
		} else if (msg->selectType == MSGSELECTTYPE_ACTION) {
			Player* p = game_->getPlayerByID(issuingClient->getPlayer());
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
			if (issuingClient == ownerClient_) {
				game_->setStatus(msg->statusType);
			} else {
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
			log("SERVER ERR: NEXTTURN message received from player " + to_string(p->getPlayerID()) + 
				", but current player is " + to_string(game_->getCurrTurnPlayer()->getPlayerID()) + "\n");
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
		game_->getPlayerByID(issuingClient->getPlayer())->useSelectedActionAt(msg->pos);
		break;
	case MSGTYPE_PLACEPROG:
	{
		// check for existence of game
		if (game_ == NULL) {
			log("SERVER ERR: cannot place program while no game in session\n");
			Message err;
			err.type = MSGTYPE_ERROR;
			strncpy_s(err.text, DEFAULT_MSG_TEXTSIZE, "ERR: you may only place a program while a game is in session", DEFAULT_MSG_TEXTSIZE);
			issuingClient->sendData(err);
			return;
		}

		// check for correct game status
		if (game_->getStatus() != GAMESTATUS_PREGAME) {
			log("SERVER ERR: cannot place program while game not in pregame\n");
			Message err;
			err.type = MSGTYPE_ERROR;
			strncpy_s(err.text, DEFAULT_MSG_TEXTSIZE, "ERR: you may only place a program during the pregame", DEFAULT_MSG_TEXTSIZE);
			issuingClient->sendData(err);
			return;
		}

		// check for spawn tile
		if (game_->getTileAt(msg->pos) != TILE_SPAWN && game_->getTileAt(msg->pos) != TILE_SPAWN2) {
			log("SERVER ERR: can only place program on spawntile\n");
			Message err;
			err.type = MSGTYPE_ERROR;
			strncpy_s(err.text, DEFAULT_MSG_TEXTSIZE, "ERR: you may only place a program on a spawn tile", DEFAULT_MSG_TEXTSIZE);
			issuingClient->sendData(err);
			return;
		}

		// remove whatever program is currently there, if any, and give it back to its user
		Program* currProg = game_->getProgramAt(msg->pos);
		if (currProg != NULL) {
			User* u = NULL;
			Iterator<Pipe*> it = clients_->getIterator();
			while (it.hasNext()) {
				Pipe* curr = it.next();
				if (game_->getPlayerByID(curr->getPlayer()) == currProg->getOwner())
					u = getUserByName(curr->getUser());
			}

			if (u == NULL)
				log("SERVER ERR: replaced program had no related user\n");

			currProg->getOwner()->setSelectedProgram(NULL);

			PROGRAM type = currProg->getType();
			game_->removeProgram(currProg->getProgramID(), currProg->getOwner()->getPlayerID(), currProg->getOwner()->getTeam());

			u->progsOwned_[type]++;
			u->progsInPlay_[type]--;

			Message m;
			m.type = MSGTYPE_PROGINVENTORY;
			m.progType = type;
			m.num = u->progsOwned_[type];
			m.clientID = msg->clientID;
			sendMessageToAllClients(m);
		}

		// place the new program
		if (msg->progType != PROGRAM_NONE) {
			Player* pl = game_->getPlayerByID(msg->playerID);
			Program* p = game_->addProgram(msg->progType, msg->playerID, pl->getTeam());
			p->addTail(msg->pos);

			pl->setSelectedProgram(p);

			User* u = getUserByName(issuingClient->getUser());
			u->progsOwned_[msg->progType]--;
			u->progsInPlay_[msg->progType]++;

			Message m;
			m.type = MSGTYPE_PROGINVENTORY;
			m.progType = msg->progType;
			m.num = u->progsOwned_[msg->progType];
			m.clientID = msg->clientID;
			sendMessageToAllClients(m);
		}
	}
	break;
	case MSGTYPE_PROGINVENTORY:
		getUserByName(issuingClient->getUser())->progsOwned_[msg->progType] += msg->num;
		saveUsers();

		Message m;
		m.type = MSGTYPE_PROGINVENTORY;
		m.clientID = 0;
		m.progType = msg->progType;
		m.num = getUserByName(issuingClient->getUser())->progsOwned_[msg->progType];
		sendMessageToClient(m, msg->clientID);
		break;
	}
}

void Server::connect(SOCKET client) {
	Pipe* newPipe = new Pipe(client);
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
	log("SERVER: Client " + to_string(client->getClientID()) + " disconnected\n");

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
		Player* p = game_->getPlayerByID(client->getPlayer());
		Team* t = game_->getTeamByID(p->getTeam());
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

		if (curr->getPlayer() != -1) {
			curr->setPlayer(-1);

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
	for (int x = game_->getLeftBound(); x < game_->getRightBound(); x++) {
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
	}

	// send players and programs
	LinkedList<Team*>* allTeams = game_->getAllTeams();
	for (int teamNum = 0; teamNum < allTeams->getLength(); teamNum++) {
		Team* currTeam = allTeams->getObjectAt(teamNum);
		LinkedList<Player*>* playerList = currTeam->getAllPlayers();
		for (int playerNum = 0; playerNum < playerList->getLength(); playerNum++) {
			Player* currPlayer = playerList->getObjectAt(playerNum);
			LinkedList<Program*>* progList = currPlayer->getProgList();
			for (int progNum = 0; progNum < progList->getLength(); progNum++) {
				Program* currProg = progList->getObjectAt(progNum);
				LinkedList<Coord*>* progCoords = currProg->getTiles();
				for (int coordNum = 0; coordNum < progCoords->getLength(); coordNum++) {
					msg.infoType = MSGINFOTYPE_PROGRAM;
					msg.teamID = currTeam->getTeamID();
					msg.playerID = currPlayer->getPlayerID();
					msg.programID = currProg->getProgramID();
					msg.progType = currProg->getType();
					msg.pos = *(progCoords->getObjectAt(coordNum));
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
		log("SERVER ERR: AI player " + to_string(p->getPlayerID()) + " doesn't have a mind, although it should\n");
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

void Server::loadUsers() {
	std::ifstream userStream;
	std::string filePath = savePath_ + "/users.dat";
	userStream.open(filePath, std::ios::in | std::ios::binary);

	if (!userStream.is_open()) {
		log("SERVER ERROR: could not open " + filePath + "\n");
	} else {
		log("SERVER: reading " + filePath + "\n");

		// read the sizes of various data types
		log("SERVER: reading constants\n");
		int8_t sizeOfInt;
		userStream.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		userStream.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		userStream.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		userStream.read((char*)&sizeOfBool, 1);
		log("SERVER: read constants: int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) +
			" double:" + to_string(sizeOfDouble) + " bool:" + to_string(sizeOfBool) + "\n");

		// read user table
		int numUsers;
		userStream.read((char*)&numUsers, sizeOfInt);
		log("SERVER: reading " + to_string(numUsers) + " users\n");
		for (int i = 0; i < numUsers; i++) {
			User* currUser = new User();

			char usernameRaw[DEFAULT_MSG_TEXTSIZE];
			userStream.read(usernameRaw, sizeOfChar*DEFAULT_MSG_TEXTSIZE);
			currUser->username_ = std::string(usernameRaw);

			char passwordRaw[DEFAULT_MSG_TEXTSIZE];
			userStream.read(passwordRaw, sizeOfChar*DEFAULT_MSG_TEXTSIZE);
			currUser->password_ = std::string(passwordRaw);

			userStream.read((char*)&currUser->numCredits_, sizeOfInt);
			userStream.read((char*)currUser->progsOwned_, sizeOfInt*PROGRAM_NUM_PROGTYPES);
			userStream.read((char*)currUser->campaignClassic_, sizeOfBool*NUM_LEVELS_CLASSIC);
			userStream.read((char*)currUser->campaignNightfall_, sizeOfBool*NUM_LEVELS_NIGHTFALL);
			userStream.read((char*)&currUser->numWins_, sizeOfInt);
			userStream.read((char*)&currUser->numLosses_, sizeOfInt);
			userStream.read((char*)&currUser->numDraws_, sizeOfInt);
			userStream.read((char*)&currUser->numResigns_, sizeOfInt);
			userStream.read((char*)&currUser->numGames_, sizeOfInt);

			users_->addFirst(currUser);
		}

		// close the file
		userStream.close();
		log("SERVER: successfully read user table\n");
	}
}

void Server::saveUsers() {
	std::ofstream userStream;
	std::string filePath = savePath_ + "/users.dat";
	userStream.open(filePath, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!userStream.is_open()) {
		log("SERVER ERROR: could not open " + filePath + "\n");
	} else {
		log("SERVER: writing " + filePath + "\n");

		// begin by writing the sizes of various data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		log("SERVER: saving constants... int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) + 
			" double:" + to_string(sizeOfDouble) + " bool:" + to_string(sizeOfBool) + "\n");
		userStream.write((char*)&sizeOfInt, 1);
		userStream.write((char*)&sizeOfChar, 1);
		userStream.write((char*)&sizeOfDouble, 1);
		userStream.write((char*)&sizeOfBool, 1);

		// write user table
		int numUsers = users_->getLength();
		userStream.write((char*)&numUsers, sizeOfInt);
		log("SERVER: writing " + to_string(numUsers) + " users\n");
		for (int i = 0; i < numUsers; i++) {
			User* currUser = users_->getObjectAt(i);
			userStream.write(currUser->username_.c_str(), sizeOfChar*DEFAULT_MSG_TEXTSIZE);
			userStream.write(currUser->password_.c_str(), sizeOfChar*DEFAULT_MSG_TEXTSIZE);

			userStream.write((char*)&currUser->numCredits_, sizeOfInt);
			userStream.write((char*)currUser->progsOwned_, sizeOfInt*PROGRAM_NUM_PROGTYPES);
			userStream.write((char*)currUser->campaignClassic_, sizeOfBool*NUM_LEVELS_CLASSIC);
			userStream.write((char*)currUser->campaignNightfall_, sizeOfBool*NUM_LEVELS_NIGHTFALL);
			userStream.write((char*)&currUser->numWins_, sizeOfInt);
			userStream.write((char*)&currUser->numLosses_, sizeOfInt);
			userStream.write((char*)&currUser->numDraws_, sizeOfInt);
			userStream.write((char*)&currUser->numResigns_, sizeOfInt);
			userStream.write((char*)&currUser->numGames_, sizeOfInt);
		}

		// close the file
		userStream.close();
		log("SERVER: successfully wrote user table\n");
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

Game* Server::getGame() {
	return game_;
}

LinkedList<User*>* Server::getUsers() {
	return users_;
}

Pipe* Server::getOwner() {
	return ownerClient_;
}

bool Server::isLocal() {
	return isLocal_;
}

int Server::getCurrentLevel() {
	return currentLevel_;
}

std::string Server::getSavePath() {
	return savePath_;
}