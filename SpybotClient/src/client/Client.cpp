#include "Standard.h"
#include "Client.h"

#include "Message.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Game.h"
#include "Program.h"
#include "NotifyScreen.h"
#include "Data.h"
#include "GameScreen.h"
#include "LobbyScreen.h"
#include "MainScreen.h"
#include "Player.h"
#include "ClientMirror.h"
#include "ChatDisplay.h"
#include "Team.h"
#include "AnimationAttack.h"
#include "AnimationSelect.h"
#include "ConnectionManager.h"

Client::Client() {
	game_ = NULL;
	player_ = NULL;
	myClientID_ = -1;
}

Client::~Client() {
	//dtor
}

void Client::processAllMessages() {
	while (_connectionManager->hasMessage()) {
		Message* curr = _connectionManager->pollMessage();
		processMessage(curr);
		delete curr;
	}
}

void Client::processMessage(Message* msg) {
	printf("CLIENT RECIEVED MSG: ");
	_printMessage(*msg);

	switch (msg->type) {
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
			_notifyScreen->addNotification("Client " + to_string(msg->clientID) + " has loaded level " + to_string(msg->levelNum));

		player_ = NULL;
		delete game_;
		game_ = NULL;
		game_ = new Game(false);
		_gameScreen->resetScreen();

		Message m;
		m.type = MSGTYPE_JOIN;
		m.clientID = myClientID_;
		_connectionManager->sendMessage(m);
		break;
	case MSGTYPE_SOUND:
		switch (msg->soundType) {
		case MSGSOUNDNAME_MOVE:
			Mix_PlayChannel(-1, _sound_move_player, msg->numRepeats);
			break;
		case MSGSOUNDNAME_DAMAGE:
			Mix_PlayChannel(-1, _sound_action_attack, msg->numRepeats);
			break;
		case MSGSOUNDNAME_HEAL:
			Mix_PlayChannel(-1, _sound_action_heal, msg->numRepeats);
			break;
		case MSGSOUNDNAME_SIZEMOD:
			Mix_PlayChannel(-1, _sound_action_heal, msg->numRepeats);
			break;
		case MSGSOUNDNAME_ZERO:
			Mix_PlayChannel(-1, _sound_action_grid_damage, msg->numRepeats);
			break;
		case MSGSOUNDNAME_ONE:
			Mix_PlayChannel(-1, _sound_action_grid_fix, msg->numRepeats);
			break;
		case MSGSOUNDNAME_PICKUPCREDIT:
			Mix_PlayChannel(-1, _sound_pickup_credit, msg->numRepeats);
			break;
		default:
			break;
		}
		break;
	case MSGTYPE_INFO:
		if (msg->infoType == MSGINFOTYPE_BKG) {
			game_->setBackground(msg->bkgType);
			_gameScreen->setBackgroundImg(msg->bkgType);
		} else if (msg->infoType == MSGINFOTYPE_TILE)
			game_->setTileAt(msg->pos, msg->tileType);
		else if (msg->infoType == MSGINFOTYPE_PROGRAM) {
			// get team
			Team* t = game_->getTeamByNum(msg->team);
			if (t == NULL) {
				t = new Team(msg->team);
				game_->getAllTeams()->addFirst(t);
			}

			// get player
			Player* p = t->getPlayerByID(msg->playerID);
			if (p == NULL) {
				p = new Player(game_, t->getTeamNum());
				p->setPlayerID(msg->playerID);
				t->getAllPlayers()->addFirst(p);
			}

			// get program
			Program* pr = p->getProgramByID(msg->programID);
			if (pr == NULL) {
				pr = new Program(msg->progType, t->getTeamNum(), msg->pos);
				pr->setProgramID(msg->programID);
				p->addProgram(pr);
			} else
				pr->addTail(msg->pos);

			game_->setProgramAt(msg->pos, pr);
		} else if (msg->infoType == MSGINFOTYPE_ACTION) {
			// TOOD: have this support creating an arbitrary action
		} else if (msg->infoType == MSGINFOTYPE_ITEM) {
			game_->setItemAt(msg->pos, msg->itemType);
		} else if (msg->infoType == MSGINFOTYPE_GAMESTATUS) {
			game_->setStatus(msg->statusType);
			_gameScreen->changeGameStatus(msg->statusType);
			_currScreen = _gameScreen;
		}
		_gameScreen->centerScreen();
		break;
	case MSGTYPE_SELECT:
		if (msg->selectType == MSGSELECTTYPE_TILE) {
			Player* p = game_->getPlayerByID(msg->playerID);
			_gameScreen->addAnimation(new AnimationSelect(msg->pos, p->getColor().r, p->getColor().g, p->getColor().b));
			p->setSelectedTile(msg->pos);
		} else if (msg->selectType == MSGSELECTTYPE_PROGRAM) {
			Player* p = game_->getPlayerByID(msg->playerID);
			p->setSelectedProgram(p->getProgramByID(msg->programID));
		} else if (msg->selectType == MSGSELECTTYPE_ACTION) {
			Program* p = game_->getPlayerByID(msg->playerID)->getProgramByID(msg->programID);
			game_->getPlayerByID(msg->playerID)->setSelectedAction(p->getActions()->getObjectAt(msg->actionID));
		}
		break;
	case MSGTYPE_JOIN:
	{
		// add notification
		_notifyScreen->addNotification("CLIENT: received player id " + to_string(msg->playerID));

		// get or create team
		Team* t = game_->getTeamByNum(msg->team);
		if (t == NULL) {
			t = new Team(msg->team);
			game_->getAllTeams()->addFirst(t);
		}

		// create player
		Player* newP = new Player(game_, msg->team);
		newP->setPlayerID(msg->playerID);
		t->getAllPlayers()->addFirst(newP);

		// if this is my client's player
		if (msg->clientID == myClientID_) {
			player_ = newP;

			Message m;
			m.type = MSGTYPE_RESYNCGAME;
			_connectionManager->sendMessage(m);
		}

		// set this clientMirror's player
		for (int i = 0; i < _connectionManager->getClientList()->getLength(); i++) {
			ClientMirror* cm = _connectionManager->getClientList()->getObjectAt(i);
			if (cm->clientID_ == msg->clientID)
				cm->player_ = newP;
		}
	}
	break;
	case MSGTYPE_CONNECT:
	{
		myClientID_ = msg->clientID;
		_notifyScreen->addNotification("Connection to server confirmed");
		_notifyScreen->addNotification("Assigned client ID " + to_string(msg->clientID));
		_mainScreen->hideIPEntry();
		_mainScreen->loginShow();
	}
	break;
	case MSGTYPE_DISCONNECT:
		// TODO: have the pipe be destroyed
		break;
	case MSGTYPE_LEAVE:
		if (_currScreen == _lobbyScreen) {
			// find the index of this clientID
			int index = -1;
			for (int i = 0; i < _connectionManager->getClientList()->getLength(); i++) {
				if (_connectionManager->getClientList()->getObjectAt(i)->clientID_ == msg->clientID)
					index = i;
			}

			// if there's a game going on, remove the player
			//game_->removePlayer(clientList_->getObjectAt(index)->player_->getPlayerID());

			// remove the client
			_notifyScreen->addNotification(_connectionManager->getClientList()->getObjectAt(index)->name_ + " has disconnected from the server");
			delete _connectionManager->getClientList()->removeObjectAt(index);
		}

		break;
	case MSGTYPE_NEXTTURN:
	{
		// set the next player
		Player* pNext = game_->getPlayerByID(msg->playerID);
		game_->setCurrTurnPlayer(pNext);
		ClientMirror* cNext = _connectionManager->getClientMirrorByPlayerID(msg->playerID);
		if (cNext != NULL)
			_gameScreen->setPlayerTurnDisplay(cNext->name_);
		else
			_gameScreen->setPlayerTurnDisplay("AI");
		_gameScreen->toggleTurnButtonShown(false);

		// if it's now my turn
		if (msg->playerID == player_->getPlayerID()) {
			// reset everyone's turns
			Iterator<Team*> itTeams = game_->getAllTeams()->getIterator();
			while (itTeams.hasNext())
				itTeams.next()->getAllPlayers()->forEach([] (Player* p) {p->endTurn(); });

			// show the "end turn" button
			_gameScreen->toggleTurnButtonShown(true);
		}
	}
	break;
	case MSGTYPE_CHAT:
	{
		std::string text = "";
		for (int i = 0; i < _connectionManager->getClientList()->getLength(); i++) {
			if (_connectionManager->getClientList()->getObjectAt(i)->clientID_ == msg->clientID)
				text += _connectionManager->getClientList()->getObjectAt(i)->name_;
		}
		text += "> " + std::string(msg->text);

		if (_currScreen == _lobbyScreen)
			_lobbyScreen->getChatDisplay()->addMessage(text);
		else if (_currScreen == _gameScreen)
			_gameScreen->getChatDisplay()->addMessage(text);
	}
	break;
	case MSGTYPE_LOGIN:
	{
		if (msg->clientID == myClientID_)
			_notifyScreen->addNotification("Logged in as " + std::string(msg->text));
		else
			_notifyScreen->addNotification(std::string(msg->text) + " has joined the game");

		ClientMirror* mirror = new ClientMirror();
		mirror->clientID_ = msg->clientID;
		if (msg->actionID == 9000)// arbitrary value
			_connectionManager->setServerOwner(mirror);
		mirror->name_ = msg->text;
		_connectionManager->getClientList()->addFirst(mirror);
		_mainScreen->loginHide();
		_mainScreen->showMainContainer();
		_currScreen = _lobbyScreen;
	}
	break;
	case MSGTYPE_CREATEUSER:
		_notifyScreen->addNotification(std::string(msg->text));
		_mainScreen->loginClear();
		break;
	case MSGTYPE_ERROR:
		_notifyScreen->addNotification(std::string(msg->text));
		break;
	case MSGTYPE_GAMECONFIG:
		if (msg->gameConfigType == MSGGAMECONFIGTYPE_COOP)
			_lobbyScreen->setGameMode(GAMEMODE_COOP);
		else if (msg->gameConfigType == MSGGAMECONFIGTYPE_FFA)
			_lobbyScreen->setGameMode(GAMEMODE_FFA);
		else if (msg->gameConfigType == MSGGAMECONFIGTYPE_TEAMDM)
			_lobbyScreen->setGameMode(GAMEMODE_TEAMDM);
		break;
	case MSGTYPE_PLACEPROG:
		// if a program was already here, remove and refund it
		Program* currProg = game_->getProgramAt(msg->pos);
		if (currProg != NULL && currProg->getOwner() == player_) {
			_progListMulti[currProg->getType()]++;
			player_->getProgList()->remove(currProg);
			game_->setProgramAt(msg->pos, NULL);
			delete currProg;
		}

		// get team
		Team* t = game_->getTeamByNum(msg->team);
		if (t == NULL) {
			t = new Team(msg->team);
			game_->getAllTeams()->addFirst(t);
		}

		// get player
		Player* p = t->getPlayerByID(msg->playerID);
		if (p == NULL) {
			p = new Player(game_, t->getTeamNum());
			p->setPlayerID(msg->playerID);
			t->getAllPlayers()->addFirst(p);
		}

		// create program
		Program* pr = new Program(msg->progType, t->getTeamNum(), msg->pos);
		_progListMulti[msg->progType]--;
		pr->setProgramID(msg->programID);
		p->addProgram(pr);
		game_->setProgramAt(msg->pos, pr);
		break;
	}
}

Player* Client::getPlayer() {
	return player_;
}

void Client::setPlayer(Player* player) {
	player_ = player;
}

Game* Client::getGame() {
	return game_;
}

void Client::setGame(Game* game) {
	game_ = game;
}

int Client::getClientID() {
	return myClientID_;
}

void Client::setClientID(int clientID) {
	myClientID_ = clientID;
}