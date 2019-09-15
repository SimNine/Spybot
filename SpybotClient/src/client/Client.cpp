#include "Standard.h"
#include "Client.h"

#include "Message.h"
#include "Global.h"
#include "MiscUtil.h"
#include "NotifyOverlay.h"
#include "Data.h"
#include "GameOverlay.h"
#include "LobbyOverlay.h"
#include "MainOverlay.h"
#include "ClientMirror.h"
#include "ChatDisplay.h"
#include "AnimationAttack.h"
#include "AnimationSelect.h"
#include "ConnectionManager.h"
#include "BackgroundOverlay.h"
#include "MapOverlay.h"
#include "Server.h"
#include "Main.h"
#include "GameMirror.h"
#include "PlayerMirror.h"
#include "ProgramMirror.h"
#include "TeamMirror.h"
#include "AnimationTileFade.h"

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
	log("CLIENT RECIEVED MSG: ");
	_printMessage(*msg);

	switch (msg->type) {
	case MSGTYPE_LOAD:
	{
		if (msg->clientID != myClientID_)
			_notifyOverlay->addNotification("Client " + to_string(msg->clientID) + " has loaded level " + to_string(msg->num));

		// reset the game
		player_ = NULL;
		delete game_;
		game_ = NULL;
		game_ = new GameMirror();
		_gameOverlay->resetScreen();

		Iterator<ClientMirror*> clientIt = _connectionManager->getClientList()->getIterator();
		while (clientIt.hasNext()) {
			clientIt.next()->player_ = NULL;
		}

		// tell the server to let this client join the game
		Message m;
		m.type = MSGTYPE_JOIN;
		_connectionManager->sendMessage(m);

		if (_server != NULL && !_progressAchievements[ACHIEVEMENT_FIRSTATTEMPTEDCYBERCRIME])
			unlockAchievement(ACHIEVEMENT_FIRSTATTEMPTEDCYBERCRIME);
	}
	break;
	case MSGTYPE_SOUND:
		switch (msg->soundType) {
		case MSGSOUNDNAME_MOVE:
			Mix_PlayChannel(-1, _sound_move_player, msg->num);
			break;
		case MSGSOUNDNAME_DAMAGE:
			Mix_PlayChannel(-1, _sound_action_attack, msg->num);
			break;
		case MSGSOUNDNAME_HEAL:
			Mix_PlayChannel(-1, _sound_action_heal, msg->num);
			break;
		case MSGSOUNDNAME_SIZEMOD:
			Mix_PlayChannel(-1, _sound_action_heal, msg->num);
			break;
		case MSGSOUNDNAME_SPEEDMOD:
			Mix_PlayChannel(-1, _sound_action_speed, msg->num);
			break;
		case MSGSOUNDNAME_ZERO:
			Mix_PlayChannel(-1, _sound_action_grid_damage, msg->num);
			break;
		case MSGSOUNDNAME_ONE:
			Mix_PlayChannel(-1, _sound_action_grid_fix, msg->num);
			break;
		case MSGSOUNDNAME_PICKUPCREDIT:
			Mix_PlayChannel(-1, _sound_pickup_credit, msg->num);
			break;
		default:
			break;
		}
		break;
	case MSGTYPE_INFO:
		if (msg->infoType == MSGINFOTYPE_BKG) {
			_gameOverlay->setBackgroundImg(msg->bkgType);
		} else if (msg->infoType == MSGINFOTYPE_TILE) {
			game_->setTileAt(msg->pos, msg->tileType);
		} else if (msg->infoType == MSGINFOTYPE_TEAM) {
			game_->addTeam(msg->teamID);
		} else if (msg->infoType == MSGINFOTYPE_PLAYER) {
			game_->addPlayer(msg->playerID, msg->teamID);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAM) {
			game_->addProgram(msg->progType, msg->programID, msg->playerID, msg->teamID);
		} else if (msg->infoType == MSGINFOTYPE_TEAMDELETE) {
			game_->removeTeam(msg->teamID);
		} else if (msg->infoType == MSGINFOTYPE_PLAYERDELETE) {
			game_->removePlayer(msg->playerID, msg->teamID);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMDELETE) {
			game_->removeProgram(msg->programID, msg->playerID, msg->teamID);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMCHANGEMAXACTIONS) {
			game_->getPlayerByID(msg->playerID)->getProgramByID(msg->programID)->setMaxActions(msg->num);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMCHANGEMAXMOVES) {
			game_->getPlayerByID(msg->playerID)->getProgramByID(msg->programID)->setMaxMoves(msg->num);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMCHANGEMAXHEALTH) {
			game_->getPlayerByID(msg->playerID)->getProgramByID(msg->programID)->setMaxHealth(msg->num);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMCHANGENUMACTIONS) {
			game_->getPlayerByID(msg->playerID)->getProgramByID(msg->programID)->setActionsLeft(msg->num);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMCHANGENUMMOVES) {
			game_->getPlayerByID(msg->playerID)->getProgramByID(msg->programID)->setMoves(msg->num);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMADDHEAD) {
			ProgramMirror* pMirr = game_->getTeamByID(msg->teamID)->getPlayerByID(msg->playerID)->getProgramByID(msg->programID);
			pMirr->addHead(msg->pos);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMADDTAIL) {
			ProgramMirror* pMirr = game_->getTeamByID(msg->teamID)->getPlayerByID(msg->playerID)->getProgramByID(msg->programID);
			pMirr->addTail(msg->pos);
		} else if (msg->infoType == MSGINFOTYPE_PROGRAMREMOVETILE) {
			ProgramMirror* pMirr = game_->getTeamByID(msg->teamID)->getPlayerByID(msg->playerID)->getProgramByID(msg->programID);
			pMirr->removeTile(msg->pos);
		} else if (msg->infoType == MSGINFOTYPE_ACTION) {
			// TODO: have this support creating an arbitrary action
		} else if (msg->infoType == MSGINFOTYPE_ITEM) {
			game_->setItemAt(msg->pos, msg->itemType);
		} else if (msg->infoType == MSGINFOTYPE_ANIM) {
			switch (msg->animType) {
			case ANIMTYPE_ACTION_ATTACK:
				_gameOverlay->addAnimation(new AnimationAttack(msg->pos, msg->num));
				break;
			case ANIMTYPE_ACTION_TILECREATE:
				_gameOverlay->addAnimation(new AnimationSelect(msg->pos, 255, 255, 255));
				break;
			case ANIMTYPE_ACTION_TILEDESTROY:
				_gameOverlay->addAnimation(new AnimationSelect(msg->pos, 0, 0, 0));
				break;
			case ANIMTYPE_PROGRAMTILEDESTROY:
			{
				SDL_Color c = game_->getProgramAt(msg->pos)->getOwner()->getColor();
				_gameOverlay->addAnimation(new AnimationTileFade(msg->pos, msg->num, c.r, c.g, c.b));
			}
			break;
			}
		} else if (msg->infoType == MSGINFOTYPE_GAMESTATUS) {
			game_->setStatus(msg->statusType);
			_gameOverlay->changeGameStatus(msg->statusType);
			_gameOverlay->updateProgramInventoryDisplay();

			if (msg->statusType == GAMESTATUS_END) {
				_gameOverlay->showWinContainer(msg->teamID);

				// increment progress
				_progressGamesPlayed++;
				if (msg->teamID == player_->getTeamID())
					_progressGamesWon++;
				else
					_progressGamesLost++;
				saveProgress();

				// if this is a local game, check for achievement unlocks
				if (_server != NULL) {
					if (!_progressAchievements[ACHIEVEMENT_FIRSTCOMMITTEDCYBERCRIME])
						unlockAchievement(ACHIEVEMENT_FIRSTCOMMITTEDCYBERCRIME);

					if (msg->teamID == player_->getTeamID() && !_progressAchievements[ACHIEVEMENT_FIRSTSUCCESSFULCYBERCRIME]) {
						unlockAchievement(ACHIEVEMENT_FIRSTSUCCESSFULCYBERCRIME);
					}
				}
			}
		} else if (msg->infoType == MSGINFOTYPE_CREDITS) {
			_client->getMyClientMirror()->credits_ = msg->num;
		}
		_gameOverlay->centerScreen();
		break;
	case MSGTYPE_SELECT:
		if (msg->selectType == MSGSELECTTYPE_TILE) {
			PlayerMirror* p = game_->getPlayerByID(msg->playerID);
			p->setSelectedTile(msg->pos);
			_gameOverlay->addAnimation(new AnimationSelect(msg->pos, p->getColor().r, p->getColor().g, p->getColor().b));
		} else if (msg->selectType == MSGSELECTTYPE_PROGRAM) {
			PlayerMirror* p = game_->getPlayerByID(msg->playerID);
			p->setSelectedProgram(p->getProgramByID(msg->programID));
		} else if (msg->selectType == MSGSELECTTYPE_ACTION) {
			PlayerMirror* pl = game_->getPlayerByID(msg->playerID);
			ProgramMirror* pr = pl->getProgramByID(msg->programID);
			pl->setSelectedAction(pr->getActions()->getObjectAt(msg->actionID));
		}
		break;
	case MSGTYPE_JOIN:
	{
		// add notification
		_notifyOverlay->addNotification("Player " + to_string(msg->playerID) + " has connected to the game");

		// get or create team
		TeamMirror* t = game_->getTeamByID(msg->teamID);
		if (t == NULL)
			game_->addTeam(msg->teamID);

		// get or create player
		PlayerMirror* p = game_->getPlayerByID(msg->playerID);
		if (p == NULL)
			game_->addPlayer(msg->playerID, msg->teamID);

		// if this is my client's player
		if (msg->clientID == myClientID_)
			player_ = p;

		// set this clientMirror's player
		for (int i = 0; i < _connectionManager->getClientList()->getLength(); i++) {
			ClientMirror* cm = _connectionManager->getClientList()->getObjectAt(i);
			if (cm->clientID_ == msg->clientID)
				cm->player_ = p;
		}

		// switch to the game overlay
		_overlayStack->removeAll();
		_overlayStack->addFirst(_gameOverlay);
	}
	break;
	case MSGTYPE_CONNECT:
	{
		myClientID_ = msg->clientID;
		_notifyOverlay->addNotification("Connection to server confirmed");
		_notifyOverlay->addNotification("Assigned client ID " + to_string(msg->clientID));

		if (_server == NULL) {
			_mainOverlay->hideIPEntry(1000);
			_mainOverlay->loginShow(1000);
		}
	}
	break;
	case MSGTYPE_DISCONNECT:
		// TODO: have the pipe be destroyed
		break;
	case MSGTYPE_LEAVE:
		if (_overlayStack->getFirst() == _lobbyOverlay) {
			// find the index of this clientID
			int index = -1;
			for (int i = 0; i < _connectionManager->getClientList()->getLength(); i++) {
				if (_connectionManager->getClientList()->getObjectAt(i)->clientID_ == msg->clientID)
					index = i;
			}

			// if there's a game going on, remove the player
			//game_->removePlayer(clientList_->getObjectAt(index)->player_->getPlayerID());

			// remove the client
			_notifyOverlay->addNotification(_connectionManager->getClientList()->getObjectAt(index)->name_ + " has disconnected from the server");
			delete _connectionManager->getClientList()->removeObjectAt(index);
		}

		break;
	case MSGTYPE_NEXTTURN:
	{
		// set the next player
		PlayerMirror* pNext = game_->getPlayerByID(msg->playerID);
		game_->setCurrTurnPlayer(pNext);
		ClientMirror* cNext = _connectionManager->getClientMirrorByPlayerID(msg->playerID);
		if (cNext != NULL)
			_gameOverlay->setPlayerTurnDisplay(cNext->name_);
		else
			_gameOverlay->setPlayerTurnDisplay("AI");
		_gameOverlay->toggleTurnButtonShown(false);

		// if it's now my turn
		if (msg->playerID == player_->getPlayerID()) {
			// reset everyone's turns
			Iterator<TeamMirror*> itTeams = game_->getAllTeams()->getIterator();
			while (itTeams.hasNext())
				itTeams.next()->getAllPlayers()->forEach([] (PlayerMirror* p) {p->endTurn(); });

			// show the "end turn" button
			_gameOverlay->toggleTurnButtonShown(true);
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

		if (_overlayStack->getFirst() == _lobbyOverlay)
			_lobbyOverlay->getChatDisplay()->addMessage(text);
		else if (_overlayStack->getFirst() == _gameOverlay)
			_gameOverlay->getChatDisplay()->addMessage(text);
	}
	break;
	case MSGTYPE_LOGIN:
	{
		if (msg->clientID == myClientID_)
			_notifyOverlay->addNotification("Logged in as " + std::string(msg->text));
		else
			_notifyOverlay->addNotification(std::string(msg->text) + " has joined the game");

		ClientMirror* mirror = new ClientMirror();
		mirror->clientID_ = msg->clientID;
		if (msg->actionID == 9000)// arbitrary value
			_connectionManager->setServerOwner(mirror);
		mirror->name_ = msg->text;
		_connectionManager->getClientList()->addFirst(mirror);

		// if this is my client's login reflection, set my client mirror
		if (mirror->clientID_ == myClientID_) {
			myClientMirror_ = mirror;
		}

		// if coming from an external server, go to the lobby screen
		if (_server == NULL) {
			_mainOverlay->loginHide(0);
			_mainOverlay->showMainContainer(0);
			_overlayStack->removeAll();
			_overlayStack->push(_backgroundOverlay);
			_overlayStack->push(_lobbyOverlay);
		} else { // otherwise, go to the map screen
			_mainOverlay->hideLocalLoginContainer(0);
			_mainOverlay->showMainContainer(0);
			_overlayStack->removeAll();
			_overlayStack->push(_mapOverlay);
		}
	}
	break;
	case MSGTYPE_CREATEUSER:
		_notifyOverlay->addNotification(std::string(msg->text));
		_mainOverlay->loginClear();
		break;
	case MSGTYPE_ERROR:
		_notifyOverlay->addNotification(std::string(msg->text));
		break;
	case MSGTYPE_GAMECONFIG:
		if (msg->gameConfigType == MSGGAMECONFIGTYPE_COOP)
			_lobbyOverlay->setGameMode(GAMEMODE_COOP);
		else if (msg->gameConfigType == MSGGAMECONFIGTYPE_FFA)
			_lobbyOverlay->setGameMode(GAMEMODE_FFA);
		else if (msg->gameConfigType == MSGGAMECONFIGTYPE_TEAMDM)
			_lobbyOverlay->setGameMode(GAMEMODE_TEAMDM);
		break;
	case MSGTYPE_PROGINVENTORY:
		if (_client->getClientID() == msg->clientID) {
			_client->getMyClientMirror()->ownedProgs_[msg->progType] = msg->num;
			_mapOverlay->updateProgramInvDisplay();
			_gameOverlay->updateProgramInventoryDisplay();
		}
		break;
	case MSGTYPE_CREDITPICKUP:
		// increment credit counters
		_progressCreditsCollected += msg->num;
		myClientMirror_->credits_ += msg->num;
		saveProgress();

		// check for credit-related achievements
		if (!_progressAchievements[ACHIEVEMENT_BITCOIN] && _progressCreditsCollected >= 1)
			unlockAchievement(ACHIEVEMENT_BITCOIN);
		else if (!_progressAchievements[ACHIEVEMENT_BITCOINCASH] && _progressCreditsCollected >= 1000)
			unlockAchievement(ACHIEVEMENT_BITCOINCASH);
		else if (!_progressAchievements[ACHIEVEMENT_RIPPLE] && _progressCreditsCollected >= 2000)
			unlockAchievement(ACHIEVEMENT_RIPPLE);
		else if (!_progressAchievements[ACHIEVEMENT_ETHEREUM] && _progressCreditsCollected >= 5000)
			unlockAchievement(ACHIEVEMENT_ETHEREUM);
		else if (!_progressAchievements[ACHIEVEMENT_LITECOIN] && _progressCreditsCollected >= 10000)
			unlockAchievement(ACHIEVEMENT_LITECOIN);
		else if (!_progressAchievements[ACHIEVEMENT_RAIBLOCKS] && _progressCreditsCollected >= 50000)
			unlockAchievement(ACHIEVEMENT_RAIBLOCKS);
		else if (!_progressAchievements[ACHIEVEMENT_DOGECOIN] && _progressCreditsCollected >= 100000)
			unlockAchievement(ACHIEVEMENT_DOGECOIN);
		else if (!_progressAchievements[ACHIEVEMENT_GARLICOIN] && _progressCreditsCollected >= 500000)
			unlockAchievement(ACHIEVEMENT_GARLICOIN);

		// show credits earned display
		_gameOverlay->showCreditPickup(msg->num);
		_gameOverlay->refreshCreditCounter();

		// remove item from local grid
		game_->setItemAt(msg->pos, ITEM_NONE);

		break;
	case MSGTYPE_LEVELUNLOCK:
		_mapOverlay->winNode(msg->num);
		break;
	}
}

PlayerMirror* Client::getPlayer() {
	return player_;
}

void Client::setPlayer(PlayerMirror* player) {
	player_ = player;
}

GameMirror* Client::getGame() {
	return game_;
}

void Client::setGame(GameMirror* game) {
	game_ = game;
}

int Client::getClientID() {
	return myClientID_;
}

void Client::setClientID(int clientID) {
	myClientID_ = clientID;
}

ClientMirror* Client::getMyClientMirror() {
	return myClientMirror_;
}

void Client::setMyClientMirror(ClientMirror* mirr) {
	myClientMirror_ = mirr;
}