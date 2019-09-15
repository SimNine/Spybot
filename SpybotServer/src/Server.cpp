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

	// TODO: undo this
	game_ = new Game();
}

Server::~Server()
{
    //dtor
}

void Server::tick(int ms)
{
    while (msgQueue_->getLength() > 0)
    {
        Message* m = msgQueue_->removeFirst();
        processMessage(m);
        delete m;
    }
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
	getClientWithID(clientID)->sendData(message);
}

void Server::sendMessageToIngameClients(Message message)
{
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext())
	{
		Pipe* curr = it.next();
		if (curr->getPlayer() != NULL)
			curr->sendData(message);
	}
}

Pipe* Server::getClientWithID(int clientID)
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
	Pipe* issuingClient = getClientWithID(msg->clientID);
	if (issuingClient == NULL && msg->clientID != 0)
	{
		printf("SERVER ERR: got a message from a client that doesn't exist\n");
		return;
	}

	// process the incoming message and then send it to all clients
	switch (msg->type)
	{
	case MSGTYPE_MOVE:
		if (game_->getStatus() != GAMESTATUS_PLAYING)
			return;

		if (game_->getCurrTurnPlayer() != getClientWithID(msg->clientID)->getPlayer())
			return;

		{
			Player* p = issuingClient->getPlayer();
			p->moveSelectedProgram(msg->pos);
		}
		break;
	case MSGTYPE_CONNECT:
		sendMessageToAllClients(*msg);

		// send the issuing client a message for each other clients currently connected
		{
			Iterator<Pipe*> it = clients_->getIterator();
			while (it.hasNext())
			{
				Pipe* curr = it.next();

				Message m;
				m.type = MSGTYPE_CONNECT;
				m.clientID = curr->getClientID();
				strcpy_s(m.text, DEFAULT_MSG_TEXTSIZE, msg->text);
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
			msg->playerID = rand();
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
			msg->playerID = p->getPlayerID();
			sendMessageToAllClients(*msg);
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
				p->setProgramID(rand());
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
		Player* p = getClientWithID(msg->clientID)->getPlayer();

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
			// step all AIs
			for (int i = 0; i < game_->getAIPlayers()->getLength(); i++)
			{
				Player* player = game_->getAIPlayers()->getObjectAt(i);
				AICore* core = game_->getAIPlayers()->getObjectAt(i)->getMind();
				if (core != NULL)
				{
					printf("SERVER ERR: AI player %i doesn't have a mind\n", player->getPlayerID());
				}
				while (!player->getDoneTurn())
				{
					core->act(1);
				}
			}

			// reset all AIs' turns
			game_->getAIPlayers()->forEach([](Player* a) {a->endTurn(); });

			// send a message saying it's the first player on the list's turn
			Message m;
			m.type = MSGTYPE_NEXTTURN;
			m.clientID = 0;
			m.playerID = game_->getHumanPlayers()->getFirst()->getPlayerID();
			sendMessageToAllClients(m);

			// set the current player in the local gamestate
			game_->setCurrTurnPlayer(game_->getPlayerByID(m.playerID));
		}
		else 
		{
			// get the next player
			int index = game_->getHumanPlayers()->getIndexOf(p);
			Player* nextPlayer = game_->getHumanPlayers()->getObjectAt(index + 1);

			// send a message saying it's the next player's turn
			Message m;
			m.type = MSGTYPE_NEXTTURN;
			m.clientID = 0;
			m.playerID = nextPlayer->getPlayerID();
			sendMessageToAllClients(m);

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

	std::thread newThread(listenToPipe, newPipe);
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
	// TODO: fix this so that the removed PlayerHuman is either scrubbed from the game or given an AI
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