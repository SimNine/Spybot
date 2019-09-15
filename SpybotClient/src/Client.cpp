#include "Standard.h"
#include "Client.h"

#include "Message.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Game.h"
#include "Program.h"
#include "NotifyScreen.h"
#include "DataContainer.h"
#include "GameScreen.h"
#include "LobbyScreen.h"
#include "MainScreen.h"
#include "Player.h"
#include "ClientMirror.h"
#include "ChatDisplay.h"

Client::Client()
{
    msgQueue_ = new LinkedList<Message*>();
	clientList_ = new LinkedList<ClientMirror*>();
	socket_ = INVALID_SOCKET;
	myClientID_ = -1;
	game_ = NULL;
	player_ = NULL;
}

Client::~Client()
{
    //dtor
}

void Client::recieveMessage(Message message)
{
	mtx.lock();
    msgQueue_->addLast(new Message(message));
	mtx.unlock();
}

void Client::processAllMessages()
{
	while (msgQueue_->getLength() > 0)
	{
		mtx.lock();
		Message* curr = msgQueue_->poll();
		mtx.unlock();
		processMessage(curr);
		delete curr;
	}
}

void Client::processMessage(Message* msg)
{
	if (msg->clientID == myClientID_)
		printf("Reflected back from server:\n");
	printMessage(*msg);

	switch (msg->type)
	{
	case MSGTYPE_ACTION:
		{
			// TODO: support more types of actions
			Player* p = game_->getPlayerByID(msg->playerID);
			p->useSelectedActionAt(msg->pos);
		}
		break;
	case MSGTYPE_MOVE:
		if (game_->getStatus() != GAMESTATUS_PLAYING)
			return;

		{
			Player* pl = game_->getPlayerByID(msg->playerID);
			Program* pr = pl->getProgramByID(msg->programID);
			pl->setSelectedProgram(pr);
			pl->moveSelectedProgram(msg->pos);
		}
		break;
	case MSGTYPE_LOAD:
		if (msg->clientID != myClientID_)
			notifyScreen->addNotification("client " + to_string(msg->clientID) + " has loaded level " + to_string(msg->levelNum));

		player_ = NULL;
		delete game_;
		game_ = NULL;
		game_ = new Game();
		gameScreen->resetScreen();

		Message m;
		m.type = MSGTYPE_JOIN;
		m.clientID = myClientID_;
		sendMessage(m);
		break;
	case MSGTYPE_SOUND:
		switch (msg->soundType)
		{
		case MSGSOUNDNAME_MOVE:
			Mix_PlayChannel(-1, dataContainer->sound_move_player, msg->numRepeats);
			break;
		case MSGSOUNDNAME_DAMAGE:
			Mix_PlayChannel(-1, dataContainer->sound_action_attack, msg->numRepeats);
			break;
		case MSGSOUNDNAME_HEAL:
			Mix_PlayChannel(-1, dataContainer->sound_action_heal, msg->numRepeats);
			break;
		case MSGSOUNDNAME_SIZEMOD:
			Mix_PlayChannel(-1, dataContainer->sound_action_heal, msg->numRepeats);
			break;
		case MSGSOUNDNAME_ZERO:
			Mix_PlayChannel(-1, dataContainer->sound_action_grid_damage, msg->numRepeats);
			break;
		case MSGSOUNDNAME_ONE:
			Mix_PlayChannel(-1, dataContainer->sound_action_grid_fix, msg->numRepeats);
			break;
		case MSGSOUNDNAME_PICKUPCREDIT:
			Mix_PlayChannel(-1, dataContainer->sound_pickup_credit, msg->numRepeats);
			break;
		default:
			break;
		}
		break;
	case MSGTYPE_INFO:
		if (msg->infoType == MSGINFOTYPE_BKG)
		{
			game_->setBackground(msg->bkgType);
			gameScreen->setBackgroundImg(msg->bkgType);
		}
		else if (msg->infoType == MSGINFOTYPE_TILE)
			game_->setTileAt(msg->pos, msg->tileType);
		else if (msg->infoType == MSGINFOTYPE_PROGRAM)
		{
			// TODO: refactor this to support an arbitrary number of teams
			if (msg->team == 0)
			{
				Player* player = game_->getPlayerByID(msg->playerID);
				if (player == NULL)
				{
					player = new Player(game_, 0);
					player->setPlayerID(msg->playerID);
					game_->getHumanPlayers()->addLast(player);
				}

				Program* prog = player->getProgramByID(msg->programID);
				if (prog == NULL)
				{
					prog = new Program(msg->progType, 0, msg->pos);
					prog->setProgramID(msg->programID);
					player->addProgram(prog);
				}
				else
					prog->addTail(msg->pos);

				game_->setProgramAt(msg->pos, prog);
			}
			else if (msg->team == 1)
			{
				Player* player = game_->getPlayerByID(msg->playerID);
				if (player == NULL)
				{
					player = new Player(game_, 1);
					player->setPlayerID(msg->playerID);
					game_->getAIPlayers()->addLast(player);
				}

				Program* prog = player->getProgramByID(msg->programID);
				if (prog == NULL)
				{
					prog = new Program(msg->progType, 1, msg->pos);
					prog->setProgramID(msg->programID);
					player->addProgram(prog);
				}
				else
					prog->addTail(msg->pos);

				game_->setProgramAt(msg->pos, prog);
			}
		}
		else if (msg->infoType == MSGINFOTYPE_ACTION)
		{
			// TOOD: have this support creating an arbitrary action
		}
		else if (msg->infoType == MSGINFOTYPE_ITEM)
			game_->setItemAt(msg->pos, msg->itemType);
		else if (msg->infoType == MSGINFOTYPE_GAMESTATUS)
		{
			game_->setStatus(msg->statusType);
			gameScreen->changeGameStatus(msg->statusType);
			currScreen = gameScreen;
		}
		gameScreen->centerScreen();
		break;
	case MSGTYPE_SELECT:
		if (msg->selectType == MSGSELECTTYPE_TILE)
		{
			game_->getPlayerByID(msg->playerID)->setSelectedTile(msg->pos);
		}
		else if (msg->selectType == MSGSELECTTYPE_PROGRAM)
		{
			Player* p = game_->getPlayerByID(msg->playerID);
			p->setSelectedProgram(p->getProgramByID(msg->programID));
		}
		else if (msg->selectType == MSGSELECTTYPE_ACTION)
		{
			printf("GOT A MSGINFOTYPE ACTION\n");
			Program* p = game_->getPlayerByID(msg->playerID)->getProgramByID(msg->programID);
			game_->getPlayerByID(msg->playerID)->setSelectedAction(p->getActions()->getFirst());
		}
		break;
	case MSGTYPE_JOIN:
		{
			// add notification
			notifyScreen->addNotification("received player id " + to_string(msg->playerID));

			// create player
			Player* newP = new Player(game_, 0);
			newP->setPlayerID(msg->playerID);
			game_->getHumanPlayers()->addLast(newP);

			// if this is my client's player
			if (msg->clientID == myClientID_)
			{
				player_ = newP;

				Message m;
				m.type = MSGTYPE_RESYNCGAME;
				sendMessage(m);
			}

			// set this clientMirror's player
			for (int i = 0; i < clientList_->getLength(); i++)
			{
				ClientMirror* cm = clientList_->getObjectAt(i);
				if (cm->clientID_ == msg->clientID)
					cm->player_ = newP;
			}
		}
		break;
	case MSGTYPE_CONNECT:
		{
			notifyScreen->addNotification(std::string(msg->text) + " has connected to the server");
			ClientMirror* mirror = new ClientMirror();
			mirror->clientID_ = msg->clientID;
			if (msg->actionID == 9000)// arbitrary value
				mirror->owner_ = true;
			mirror->name_ = msg->text;
			clientList_->addFirst(mirror);
		}
		break;
	case MSGTYPE_DISCONNECT:
		// TODO: have the pipe be destroyed
		break;
	case MSGTYPE_LEAVE:
		if (currScreen == lobbyScreen)
		{
			// find the index of this clientID
			int index = -1;
			for (int i = 0; i < clientList_->getLength(); i++)
			{
				if (clientList_->getObjectAt(i)->clientID_ == msg->clientID)
					index = i;
			}

			notifyScreen->addNotification(clientList_->getObjectAt(index)->name_ + " has disconnected from the server");
			delete clientList_->removeObjectAt(index);
		}

		break;
	case MSGTYPE_NEXTTURN:
		{
			// set the next player
			Player* pNext = game_->getPlayerByID(msg->playerID);
			game_->setCurrTurnPlayer(pNext);
			ClientMirror* cNext = getClientMirrorByPlayerID(msg->playerID);
			if (cNext != NULL)
				gameScreen->setPlayerTurnDisplay(cNext->name_);
			else
				gameScreen->setPlayerTurnDisplay("AI");
			gameScreen->toggleTurnButtonShown(false);

			// if it's now my turn
			if (msg->playerID == player_->getPlayerID())
			{
				// reset everyone's turns
				game_->getHumanPlayers()->forEach([](Player* ph) {ph->endTurn(); });
				game_->getAIPlayers()->forEach([](Player* pc) {pc->endTurn(); });

				// show the "end turn" button
				gameScreen->toggleTurnButtonShown(true);
			}
		}
		break;
	case MSGTYPE_TEXT:
		{
			std::string text = "";
			for (int i = 0; i < clientList_->getLength(); i++)
			{
				if (clientList_->getObjectAt(i)->clientID_ == msg->clientID)
					text += clientList_->getObjectAt(i)->name_;
			}
			text += "> " + std::string(msg->text);

			if (currScreen == lobbyScreen)
				lobbyScreen->getChatDisplay()->addMessage(text);
			else if (currScreen == gameScreen)
				gameScreen->getChatDisplay()->addMessage(text);
		}
		break;
	}
}

void Client::connectIP(std::string IP)
{
	if (socket_ != INVALID_SOCKET)
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
		printf("getaddrinfo failed with error: %d\n", iResult);
	}

	// Attempt to connect to an address until one succeeds
	struct addrinfo* ptr = NULL;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		socket_ = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (socket_ == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
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
		printf("Unable to connect to server!\n");
	}

	// Receive until the peer closes the connection
	printf("CLIENT: attempting to connect to server \"%s\"\n", IP.c_str());
	std::thread newThread(&Client::listen, this);
	newThread.detach();
}

void Client::disconnect()
{
	// go back to the main screen
	currScreen = mainScreen;
	notifyScreen->addNotification("disconnected from server");

	// shut down the connection and close the socket
	if (shutdown(socket_, SD_SEND) == SOCKET_ERROR) {
		printf("CLIENT ERR: shutdown failed with error: %d\n", WSAGetLastError());
	}
	closesocket(socket_);

	if (game_ != NULL)
	{
		player_ = NULL;
		delete game_;
		game_ = NULL;
	}
	while (msgQueue_->getLength() > 0)
		delete msgQueue_->poll();
	while (clientList_->getLength() > 0)
		delete clientList_->poll();
	socket_ = INVALID_SOCKET;
	myClientID_ = -1;
}

void Client::listen()
{
	int bytesRead;

	char readBuffer[DEFAULT_MSGSIZE];
	int readBufferLength = DEFAULT_MSGSIZE;

	// listen for the first message, which should be a connection confirmation
	bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
	if (bytesRead > 0) {
		Message m = deserializeMessage(readBuffer);
		if (m.type == MSGTYPE_CONNECT)
		{
			myClientID_ = m.clientID;
			strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, username.c_str(), DEFAULT_MSG_TEXTSIZE);
			printf("CLIENT: received connection confirmation from server, acquired client ID %i\n", myClientID_);

			this->sendMessage(m);
		}
		else
		{
			printf("CLIENT ERR: first message recieved from server was not connection confirmation\n");
			disconnect();
			return;
		}
	}
	else if (bytesRead == 0)
	{
		printf("CLIENT: recieved shutdown from server\n");
	}
	else {
		printf("CLIENT ERR: recieve from server failed with error: %d\n", WSAGetLastError());
	}

	// receive until the peer shuts down the connection or there's a read error
	do {
		bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
		if (bytesRead > 0) {
			Message m = deserializeMessage(readBuffer);
			recieveMessage(m);
		}
		else if (bytesRead == 0)
			printf("CLIENT: recieved shutdown from server\n");
		else {
			printf("CLIENT ERR: recieve from server failed with error: %d\n", WSAGetLastError());
		}
	} while (bytesRead > 0);

	disconnect();
	return;
}

void Client ::sendMessage(Message m)
{
	char buffer[DEFAULT_MSGSIZE];
	m.clientID = myClientID_;
	serializeMessage(buffer, m);

	int bytesSent = send(socket_, buffer, DEFAULT_MSGSIZE, 0);
	if (bytesSent == SOCKET_ERROR) {
		printf("CLIENT ERR: send failed with error: %d\n", WSAGetLastError());
		closesocket(socket_);
	}
}

Player* Client::getPlayer()
{
	return player_;
}

void Client::setPlayer(Player* player)
{
	player_ = player;
}

Game* Client::getGame()
{
	return game_;
}

void Client::setGame(Game* game)
{
	game_ = game;
}

LinkedList<ClientMirror*>* Client::getClientList()
{
	return clientList_;
}

ClientMirror* Client::getClientMirrorByClientID(int clientID)
{
	Iterator<ClientMirror*> it = clientList_->getIterator();
	while (it.hasNext())
	{
		ClientMirror* curr = it.next();
		if (curr->clientID_ == clientID)
			return curr;
	}

	return NULL;
}

ClientMirror* Client::getClientMirrorByPlayerID(int playerID)
{
	Iterator<ClientMirror*> it = clientList_->getIterator();
	while (it.hasNext())
	{
		ClientMirror* curr = it.next();
		if (curr->player_->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
}