#include "Standard.h"
#include "Server.h"

#include "Game.h"
#include "Message.h"
#include "Pipe.h"
#include "MiscUtil.h"
#include "Program.h"
#include "Player.h"
#include "AICore.h"

Server::Server()
{
    clients_ = new LinkedList<Pipe*>();
    msgQueue_ = new LinkedList<Message*>();
}

Server::~Server()
{
    //dtor
}

void Server::tick(int ms)
{
	mtx.lock();
    while (msgQueue_->getLength() > 0)
    {
        Message* m = msgQueue_->removeFirst();
        processMessage(m);
        delete m;
    }
	mtx.unlock();
}

void Server::sendMessageToAllClients(Message message)
{
    Iterator<Pipe*> it = clients_->getIterator();
    while (it.hasNext())
    {
        it.next()->sendData(message);
    }
}

void Server::sendMessageToClient(Message message, int clientID)
{
	getClientByID(clientID)->sendData(message);
}

void Server::sendMessageToAllClientsExcept(Message message, int clientID)
{
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext())
	{
		Pipe* curr = it.next();
		if (curr->getClientID() != clientID)
			curr->sendData(message);
	}
}

Pipe* Server::getClientByID(int clientID)
{
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext())
	{
		Pipe* curr = it.next();
		if (curr->getClientID() == clientID)
		{
			return curr;
		}
	}

	// if a client with a matching ID wasn't found, return null
	return NULL;
}

void Server::recieveMessage(Message message)
{
	mtx.lock();
    msgQueue_->addLast(new Message(message));
	mtx.unlock();
}

void Server::processMessage(Message* msg)
{
	printMessage(*msg);

	// get the pipe of the issuer
	Pipe* issuingClient = getClientByID(msg->clientID);
	if (issuingClient == NULL && msg->clientID != 0)
	{
		printf("SERVER ERR: got a message from a client that doesn't exist\n");
		return;
	}

	// process the incoming message and then send it to all clients
	switch (msg->type)
	{
	case MSGTYPE_TEXT:
		// check if usable as a command
		if (msg->text[0] == '/' && msg->clientID == clients_->getLast()->getClientID())
		{
			std::thread doCommand(&Server::processCommand, this, std::string(&msg->text[1]));
			doCommand.detach();
		}
		else
			// forward to all clients
			sendMessageToAllClients(*msg);
		break;
	case MSGTYPE_MOVE:
		if (game_->getStatus() != GAMESTATUS_PLAYING)
			return;

		if (game_->getCurrTurnPlayer() != getClientByID(msg->clientID)->getPlayer())
			return;

		{
			Player* p = issuingClient->getPlayer();
			p->moveSelectedProgram(msg->pos);
		}
		break;
	case MSGTYPE_CONNECT:
		// if there is no owner client, make this it
		if (ownerClient_ == NULL)
			ownerClient_ = getClientByID(msg->clientID);

		// set this client's name
		getClientByID(msg->clientID)->setName(msg->text);

		// send a connect message for the new client to all *other* clients
		sendMessageToAllClientsExcept(*msg, msg->clientID);

		// send the issuing client a message for every client currently connected
		{
			Iterator<Pipe*> it = clients_->getIterator();
			while (it.hasNext())
			{
				Pipe* curr = it.next();

				// send the newly connected client a list of all clients
				Message m;
				m.type = MSGTYPE_CONNECT;
				m.clientID = curr->getClientID();
				strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, curr->getName().c_str(), DEFAULT_MSG_TEXTSIZE);
				if (curr == ownerClient_)
					m.actionID = 9000; // an arbitrary value to indicate this is the owner client
				else
					m.actionID = 0;
				sendMessageToClient(m, msg->clientID);
			}
		}
		break;
    case MSGTYPE_LOAD:
		if (game_ != NULL)
		{
			delete game_;
		}

        game_ = new Game("levels/classic/" + to_string(msg->levelNum) + ".urf");
		sendMessageToAllClients(*msg);
        break;
    case MSGTYPE_RESYNCGAME:
        resyncClient(msg->clientID);
        break;
    case MSGTYPE_JOIN:
		if (game_ == NULL)
		{
			printf("SERVER ERR: client %i tried to join a game that doesn't exist\n", msg->clientID);
			return;
		}
        else if (issuingClient->getPlayer() == NULL)
        {
			printf("SERVER: client %i has joined the current game\n", msg->clientID);

			// create a new player in the local game, assign it a unique playerID
            Player* newPlayer = new Player(game_, 0);
			msg->playerID = issuingClient->getClientID();
			newPlayer->setPlayerID(msg->playerID);
            game_->getHumanPlayers()->addLast(newPlayer);
			issuingClient->setPlayer(newPlayer);

			// send this client's new unique playerID to every client
			sendMessageToAllClients(*msg);
        }
        else
        {
            printf("SERVER ERR: client %i trying to join an already-joined game\n", msg->clientID);
			return;
        }
        break;
    case MSGTYPE_SELECT:
		{
			Player* p = issuingClient->getPlayer();
			p->setSelectedTile(msg->pos);
			break;
		}
	case MSGTYPE_LEAVE:
		{
			disconnect(issuingClient);
			break;
		}
	case MSGTYPE_INFO:
		if (msg->infoType == MSGINFOTYPE_GAMESTATUS)
		{
			game_->setStatus(msg->statusType);
		}
		else if (msg->infoType == MSGINFOTYPE_PROGRAM)
		{
			//printf("trying to place prog from clientID %i, playerID %i\n", msg->clientID, msg->playerID);

			// check for correct game status
			if (game_->getStatus() != GAMESTATUS_PLACING_PROGRAMS)
				return;

			// check for spawn tile
			if (game_->getTileAt(msg->pos) != TILE_SPAWN && game_->getTileAt(msg->pos) != TILE_SPAWN2)
				return;

			// deal with cases
			if (msg->progType == PROGRAM_NONE)
			{
				Program* p = game_->getProgramAt(msg->pos);
				if (p == NULL)
					return;

				game_->getPlayerByID(p->getOwner()->getPlayerID())->getProgList()->remove(p);
				game_->setProgramAt(msg->pos, NULL);
				delete p;

				sendMessageToAllClients(*msg);
			}
			else
			{
				Program* p = new Program(msg->progType, 0, msg->pos);
				p->setProgramID(randInt());
				game_->getPlayerByID(msg->playerID)->addProgram(p);
				game_->setProgramAt(msg->pos, p);

				msg->team = 0;
				msg->programID = p->getProgramID();
				sendMessageToAllClients(*msg);
			}
		}
		break;
	case MSGTYPE_NEXTTURN:
		// get the player that just ended its turn
		Player* p = getClientByID(msg->clientID)->getPlayer();

		// check if this message actually came from the current player
		if (p->getPlayerID() != game_->getCurrTurnPlayer()->getPlayerID())
		{
			printf("SERVER ERR: NEXTTURN message received from player %i, but current player is %i\n", p->getPlayerID(), game_->getCurrTurnPlayer()->getPlayerID());
			return;
		}
		
		// reset this player's moves
		p->endTurn();

		// if this is the last player in the list
		if (game_->getHumanPlayers()->getIndexOf(p) == game_->getHumanPlayers()->getLength() - 1)
		{
			// release thread to handle AI moves so that server can continue receiving messages
			std::thread enemies(&Server::processAITurns, this);
			enemies.detach();
		}
		else 
		{
			// get the next player
			int index = game_->getHumanPlayers()->getIndexOf(p);
			Player* nextPlayer = game_->getHumanPlayers()->getObjectAt(index + 1);

			// set the current player in the gamestate
			game_->setCurrTurnPlayer(nextPlayer);
		}
		break;
    }
}

void Server::connect(SOCKET client)
{
	Pipe* newPipe = new Pipe(client, this);
    clients_->addFirst(newPipe);

	std::thread newThread(&Pipe::listenData, newPipe);
	newThread.detach();
}

void Server::disconnect(Pipe* client)
{
	if (!client->isDead())
	{
		// kill the thread formally so that death can begin; the pipe will call this method again
		client->kill();
		return;
	}

	// let the server know
	printf("Client %i disconnected\n", client->getClientID());

	// remove client from list
	clients_->remove(client);

	// remove the player
	Player* p = client->getPlayer();
	game_->getHumanPlayers()->remove(p);

	// send leave message to all connected clients
	Message msg;
	msg.type = MSGTYPE_LEAVE;
	msg.clientID = client->getClientID();
	sendMessageToAllClients(msg);
}

void Server::quitAll()
{
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext())
	{
		Pipe* curr = it.next();

		if (curr->getPlayer() != NULL)
		{
			curr->setPlayer(NULL);

			Message m;
			m.type = MSGTYPE_LEAVE;
			m.clientID = curr->getClientID();
			sendMessageToAllClients(m);
		}
	}
}

void Server::resyncAll()
{
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext())
	{
		resyncClient(it.next()->getClientID());
	}
}

void Server::resyncClient(int clientID)
{
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
        for (int y = game_->getTopBound(); y < game_->getBottomBound(); y++)
        {
            msg.infoType = MSGINFOTYPE_TILE;
            msg.pos = Coord{x, y};
            msg.tileType = game_->getTileAt(msg.pos);
            sendMessageToClient(msg, clientID);

            if (game_->getItemAt(msg.pos) != ITEM_NONE)
            {
                msg.infoType = MSGINFOTYPE_ITEM;
                msg.itemType = game_->getItemAt(msg.pos);
                sendMessageToClient(msg, clientID);
            }
        }

    // send enemy players and programs
    LinkedList<Player*>* playerList = game_->getAIPlayers();
    for (int i = 0; i < playerList->getLength(); i++)
    {
        Player* currPlayer = playerList->getObjectAt(i);
        LinkedList<Program*>* progList = currPlayer->getProgList();
        for (int j = 0; j < progList->getLength(); j++)
        {
            Program* currProg = progList->getObjectAt(j);
            LinkedList<Coord*>* progCoords = currProg->getTiles();
            for (int k = 0; k < progCoords->getLength(); k++)
            {
                msg.infoType = MSGINFOTYPE_PROGRAM;
                msg.playerID = currPlayer->getPlayerID();
				msg.programID = currProg->getProgramID();
                msg.progType = currProg->getType();
                msg.pos = *(progCoords->getObjectAt(k));
                msg.team = 1;
                sendMessageToClient(msg, clientID);
            }
        }
    }

    // send friendly players and programs
    LinkedList<Player*>* playerHumanList = game_->getHumanPlayers();
    for (int i = 0; i < playerHumanList->getLength(); i++)
    {
        Player* currPlayer = playerHumanList->getObjectAt(i);
        LinkedList<Program*>* progList = currPlayer->getProgList();
        for (int j = 0; j < progList->getLength(); j++)
        {
            Program* currProg = progList->getObjectAt(j);
            LinkedList<Coord*>* progCoords = currProg->getTiles();
            for (int k = 0; k < progCoords->getLength(); k++)
            {
                msg.infoType = MSGINFOTYPE_PROGRAM;
                msg.playerID = currPlayer->getPlayerID();
                msg.programID = currProg->getProgramID();
                msg.progType = currProg->getType();
                msg.pos = *(progCoords->getObjectAt(k));
                msg.team = 0;
                sendMessageToClient(msg, clientID);
            }
        }
    }

	// send the current game status
	msg.type = MSGTYPE_INFO;
	msg.infoType = MSGINFOTYPE_GAMESTATUS;
	msg.statusType = game_->getStatus();
	sendMessageToClient(msg, clientID);
}

LinkedList<Pipe*>* Server::getClientList()
{
	return clients_;
}

void Server::processAITurns()
{
	// step all AIs
	for (int i = 0; i < game_->getAIPlayers()->getLength(); i++)
	{
		// pick the next AI player
		Player* player = game_->getAIPlayers()->getObjectAt(i);
		AICore* core = game_->getAIPlayers()->getObjectAt(i)->getMind();

		// set the next AI player
		game_->setCurrTurnPlayer(player);

		// execute this player's moves and actions
		if (core == NULL)
		{
			printf("SERVER ERR: AI player %i doesn't have a mind\n", player->getPlayerID());
		}
		else
			while (!player->getDoneTurn())
			{
				core->act(1);
				Sleep(200);
			}
	}

	// reset all AIs' turns
	game_->getAIPlayers()->forEach([](Player* a) {a->endTurn(); });

	// set the current player to the first human player
	game_->setCurrTurnPlayer(game_->getHumanPlayers()->getFirst());
}

void Server::processCommand(std::string cmd)
{
	// tokenize
	// (gotta add a newline because that's how the tokenizer works)
	char* tokens[1024];
	int numTokens = tokenize(tokens, (cmd + "\n").c_str(), ' ');

	// process tokens
	if (numTokens == 0)
	{
		printf(">> no command entered\n");
	}
	else if (strcmp(tokens[0], "listclients") == 0)
	{
		printf(">> connected clientIDs:\n");
		Iterator<Pipe*> it = clients_->getIterator();
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
		}

		int clientID = atoi(tokens[1]);
		if (clientID == 0)
		{
			printf(">> cannot kick clientID 0: is server\n");
			destroyTokens(tokens);
		}
		printf(">> attempting to kick clientID %i\n", clientID);

		Message m;
		m.type = MSGTYPE_LEAVE;
		m.clientID = clientID;
		recieveMessage(m);
	}
	else if (strcmp(tokens[0], "load") == 0)
	{
		if (numTokens != 2)
		{
			printf(">> wrong number of arguments:\nload (levelnum)\n");
			printf(">> load (levelnum)\n");
			destroyTokens(tokens);
		}

		int levelNum = atoi(tokens[1]);

		Message m;
		m.type = MSGTYPE_LOAD;
		m.levelNum = levelNum;
		m.clientID = 0;
		recieveMessage(m);
	}
	else if (strcmp(tokens[0], "help") == 0)
	{
		printf(">> Available commands:\n");
		printf(">> listclients\n");
		printf(">> kick (clientID)\n");
		printf(">> load (levelnum)\n");
		printf(">> say (string)\n");
		printf(">> listplayers\n");
	}
	else if (strcmp(tokens[0], "say") == 0)
	{
		std::string ret = "";
		for (int i = 1; i < numTokens; i++)
			ret += std::string(tokens[i]) + " ";

		Message m;
		m.type = MSGTYPE_TEXT;
		m.clientID = 0;
		strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, ret.c_str(), DEFAULT_MSG_TEXTSIZE);
		recieveMessage(m);
	}
	else if (strcmp(tokens[0], "listplayers") == 0)
	{
		Iterator<Pipe*> ith = clients_->getIterator();
		while (ith.hasNext())
		{
			Pipe* curr = ith.next();
			printf(">> player %i - client %i\n", curr->getPlayer()->getPlayerID(), curr->getClientID());
		}

		Iterator<Player*> itc = game_->getAIPlayers()->getIterator();
		while (itc.hasNext())
		{
			Player* curr = itc.next();
			printf(">> player %i - AI\n", curr->getPlayerID());
		}
	}
	else
	{
		printf(">> \"%s\" is not a valid command\n", cmd.c_str());
		printf(">> use \"help\" for a list of commands\n");
	}

	// destroy tokens
	destroyTokens(tokens);
}

void Server::processCommandLoop()
{
	while (true)
	{
		// get the line
		std::string buffer;
		getline(std::cin, buffer);

		// process it
		processCommand(buffer);
	}

	printf("parser loop existed for some reason\n");
}