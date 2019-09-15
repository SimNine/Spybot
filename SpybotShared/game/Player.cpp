#include "Standard.h"
#include "Player.h"

#include "Program.h"
#include "Game.h"
#include "Message.h"
#include "Global.h"
#include "MiscUtil.h"
#include "ProgramAction.h"
#include "Server.h"
#include "Pipe.h"
#include "User.h"

Player::Player(Game* g, int t) {
	game_ = g;
	team_ = t;
	doneTurn_ = false;
	selectedTile_ = { -1, -1 };
	selectedProgram_ = NULL;
	progsOwned_ = new LinkedList<Program*>();
	color_ = { (Uint8)rand(), (Uint8)rand(), (Uint8)rand(), (Uint8)rand() };
	brain_ = NULL;
}

Player::~Player() {
	while (progsOwned_->getLength() > 0) {
		Program* p = progsOwned_->poll();
		delete p;
	}
	delete progsOwned_;
}

void Player::setSelectedProgram(Program* p) {
	if (p == NULL)
		calculateProgramDist(NULL);
	else if
		(p->getTeam() != team_) calculateProgramDist(NULL);
	else
		calculateProgramDist(p);

	setSelectedAction(NULL);
	selectedProgram_ = p;

	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_PROGRAM;
	m.clientID = 0;
	m.playerID = playerID_;
	if (p != NULL)
		m.programID = p->getProgramID();
	else
		m.programID = -1;
	_server->sendMessageToAllClients(m);
}

void Player::moveSelectedProgram(Coord pos) {
	// check for validity
	if (!game_->isTiled(pos)) return;

	// if the given space is adjacent to the selected program,
	// and the selected program has moves left
	if (selectedProgDist_[pos.x][pos.y] == 1 && selectedProgram_->getMoves() > 0) {
		// delete the tail of this program if it's at max health
		if (selectedProgram_->getHealth() == selectedProgram_->getMaxHealth() &&
			game_->getProgramAt(pos) != selectedProgram_) {
			Coord temp = selectedProgram_->getTail();
			game_->setProgramAt(temp, NULL);
		}

		// move the program
		game_->setProgramAt(pos, selectedProgram_);
		selectedProgram_->moveTo(pos);
		selectedTile_ = selectedProgram_->getHead();
		calculateProgramDist(selectedProgram_);

		Message msg;
		// move the program for the client
		msg.type = MSGTYPE_MOVE;
		msg.clientID = 0;
		msg.playerID = playerID_;
		msg.programID = selectedProgram_->getProgramID();
		msg.pos = pos;
		_server->sendMessageToAllClients(msg);

		// send the move sound to the client
		msg.type = MSGTYPE_SOUND;
		msg.soundType = MSGSOUNDNAME_MOVE;
		msg.numRepeats = 0;
		_server->sendMessageToAllClients(msg);

		// if there is a credit here, pick it up
		if (game_->getItemAt(pos) == ITEM_CREDIT && team_ == 0) {
			game_->setItemAt(pos, ITEM_NONE);

			msg.soundType = MSGSOUNDNAME_PICKUPCREDIT;
			_server->sendMessageToAllClients(msg);

			Pipe* recievingClient = NULL;
			Iterator<Pipe*> it = _server->getClientList()->getIterator();
			while (it.hasNext()) {
				Pipe* curr = it.next();
				if (curr->getPlayer() == playerID_) {
					msg.type = MSGTYPE_CREDITPICKUP;
					msg.pos = pos;
					msg.actionID = rand() % 300;
					_server->getUserByName(curr->getUser())->numCredits_ += msg.actionID;
					_server->sendMessageToClient(msg, curr->getClientID());
					_server->saveUsers();
					break;
				}
			}
		}
	}
}

void Player::moveSelectedProgramBy(Coord disp) {
	Coord n = selectedProgram_->getCore() + disp;
	moveSelectedProgram(n);
}

bool Player::canSelectedProgramMoveTo(Coord pos) {
	// in case something breaks
	if (selectedProgram_ == NULL)
		return false;

	// in case this isn't your program
	if (selectedProgram_->getOwner() != this)
		return false;

	//otherwise...
	if (selectedProgDist_[pos.x][pos.y] == 1)
		return true;
	else
		return false;
}

bool Player::canSelectedProgramMoveBy(Coord disp) {
	// in case something breaks
	if (selectedProgram_ == NULL)
		return false;

	//otherwise...
	if (selectedProgDist_[selectedProgram_->getCore().x + disp.x][selectedProgram_->getCore().y + disp.y] >= 1)
		return true;
	else
		return false;
}

Program* Player::getSelectedProgram() {
	return selectedProgram_;
}

int Player::getSelectedProgramDist(Coord pos) {
	return selectedProgDist_[pos.x][pos.y];
}

int Player::getSelectedProgramDistAll(Coord pos) {
	return selectedProgDistAll_[pos.x][pos.y];
}

Coord Player::getFarthestTile(Program* p) {
	LinkedList<Coord*> ll = LinkedList<Coord*>();
	for (int x = game_->getLeftBound(); x < game_->getRightBound(); x++)
		for (int y = game_->getLeftBound(); y < game_->getRightBound(); y++)
			if (game_->getProgramAt({ x, y }) == p)
				ll.addFirst(new Coord{ x, y });

	Coord h = p->getCore();
	Coord f = p->getCore();
	while (ll.getLength() > 0) {
		Coord* currP = ll.poll();
		Coord curr = *currP;
		delete currP;

		int farDist = dist(f, h);
		int currDist = dist(curr, h);

		if (currDist > farDist)
			f = curr;
	}

	return f;
}

void Player::calculateProgramDist(Program* p) {
	for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++) {
		selectedProgDist_[x][y] = -1;
		selectedProgDistAll_[x][y] = -1;
	}

	if (p == NULL)
		return;

	Coord h = p->getCore();
	selectedProgDist_[h.x][h.y] = 0;
	selectedProgDistAll_[h.x][h.y] = 0;

	LinkedList<Coord*> ll = LinkedList<Coord*>();
	ll.addFirst(new Coord(h));

	// for each pair in the list
	while (ll.getLength() > 0) {
		Coord* currP = ll.poll();
		Coord curr = *currP;
		delete currP;

		int dCurr = selectedProgDistAll_[curr.x][curr.y];

		// check tile to the right
		if (curr.x + 1 < 200 &&
			game_->getTileAt({ curr.x + 1, curr.y }) != TILE_NONE &&
			(game_->getProgramAt({ curr.x + 1, curr.y }) == NULL || game_->getProgramAt({ curr.x + 1, curr.y }) == p) &&
			selectedProgDistAll_[curr.x + 1][curr.y] == -1) {
			if (dCurr < p->getMoves())
				selectedProgDist_[curr.x + 1][curr.y] = dCurr + 1;
			selectedProgDistAll_[curr.x + 1][curr.y] = dCurr + 1;
			ll.addLast(new Coord{ curr.x + 1, curr.y });
		}

		// check tile to the left
		if (curr.x - 1 >= 0 &&
			game_->getTileAt({ curr.x - 1, curr.y }) != TILE_NONE &&
			(game_->getProgramAt({ curr.x - 1, curr.y }) == NULL || game_->getProgramAt({ curr.x - 1, curr.y }) == p) &&
			selectedProgDistAll_[curr.x - 1][curr.y] == -1) {
			if (dCurr < p->getMoves())
				selectedProgDist_[curr.x - 1][curr.y] = dCurr + 1;
			selectedProgDistAll_[curr.x - 1][curr.y] = dCurr + 1;
			ll.addLast(new Coord{ curr.x - 1, curr.y });
		}

		// check tile below
		if (curr.y + 1 < 200 &&
			game_->getTileAt({ curr.x, curr.y + 1 }) != TILE_NONE &&
			(game_->getProgramAt({ curr.x, curr.y + 1 }) == NULL || game_->getProgramAt({ curr.x, curr.y + 1 }) == p) &&
			selectedProgDistAll_[curr.x][curr.y + 1] == -1) {
			if (dCurr < p->getMoves())
				selectedProgDist_[curr.x][curr.y + 1] = dCurr + 1;
			selectedProgDistAll_[curr.x][curr.y + 1] = dCurr + 1;
			ll.addLast(new Coord{ curr.x, curr.y + 1 });
		}

		// check tile above
		if (curr.y - 1 >= 0 &&
			game_->getTileAt({ curr.x, curr.y - 1 }) != TILE_NONE &&
			(game_->getProgramAt({ curr.x, curr.y - 1 }) == NULL || game_->getProgramAt({ curr.x, curr.y - 1 }) == p) &&
			selectedProgDistAll_[curr.x][curr.y - 1] == -1) {
			if (dCurr < p->getMoves())
				selectedProgDist_[curr.x][curr.y - 1] = dCurr + 1;
			selectedProgDistAll_[curr.x][curr.y - 1] = dCurr + 1;
			ll.addLast(new Coord{ curr.x, curr.y - 1 });
		}
	}
}

void Player::addProgram(Program* p) {
	if (progsOwned_->contains(p))
		return;

	progsOwned_->addLast(p);
	p->setOwner(this);
}

void Player::setSelectedTile(Coord pos) {
	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_TILE;
	m.clientID = 0;
	m.playerID = playerID_;

	if (game_->isOOB(pos)) {
		setSelectedProgram(NULL);
		selectedTile_ = { -1, -1 };

		m.pos = pos;
		_server->sendMessageToAllClients(m);
		return;
	}

	setSelectedProgram(game_->getProgramAt(pos));

	if (selectedProgram_ == NULL) {
		selectedTile_ = pos;
		m.pos = pos;
		_server->sendMessageToAllClients(m);
	} else {
		selectedTile_ = selectedProgram_->getHead();
		m.pos = selectedProgram_->getHead();
		_server->sendMessageToAllClients(m);
	}
}

Coord Player::getSelectedTile() {
	return selectedTile_;
}

void Player::endTurn() {
	doneTurn_ = false;

	Iterator<Program*> it = progsOwned_->getIterator();
	while (it.hasNext())
		it.next()->endTurn();

	setSelectedTile({ -1, -1 });
}

bool Player::getDoneTurn() {
	return doneTurn_;
}

LinkedList<Program*>* Player::getProgList() {
	return progsOwned_;
}

int Player::getTeam() {
	return team_;
}

void Player::setSelectedAction(ProgramAction* pa) {
	for (int x = 0; x < 200; x++)
		for (int y = 0; y < 200; y++)
			selectedActionDist_[x][y] = -1;

	if (selectedProgram_ == NULL || pa == NULL) {
		selectedAction_ = NULL;
		return;
	}

	Coord center = selectedProgram_->getCore();
	for (int x = -pa->range_; x <= pa->range_; x++) {
		int rngLeft = pa->range_ - abs(x);
		for (int y = -rngLeft; y <= rngLeft; y++)
			if (game_->isTiled(center + Coord{ x, y }))
				selectedActionDist_[center.x + x][center.y + y] = dist(center, center + Coord{ x, y });
	}
	selectedActionDist_[center.x][center.y] = 0;

	selectedAction_ = pa;

	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_ACTION;
	m.clientID = 0;
	m.playerID = playerID_;
	m.programID = selectedProgram_->getProgramID();
	m.actionID = selectedProgram_->getActions()->getIndexOf(pa);
	_server->sendMessageToAllClients(m);
}

ProgramAction* Player::getSelectedAction() {
	return selectedAction_;
}

int Player::getSelectedActionDist(Coord pos) {
	return selectedActionDist_[pos.x][pos.y];
}

void Player::useSelectedActionAt(Coord pos) {
	if (selectedAction_ == NULL || game_->isOOB(pos) || selectedProgram_->getActionsLeft() <= 0)
		return;

	Program* tgtProg = game_->getProgramAt(pos);

	switch (selectedAction_->type_) {
	case ACTIONTYPE_DAMAGE:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() != team_) {
			for (int i = 0; i < selectedAction_->power_; i++) {
				Coord* curr = tgtProg->popTail();
				if (curr == NULL)
					break;
				else {
					SDL_Color c = tgtProg->getOwner()->getColor();
					game_->setProgramAt(*curr, NULL);
				}
			}
		}
		if (tgtProg->getHealth() <= 0) {
			Player* owner = tgtProg->getOwner();
			owner->getProgList()->remove(tgtProg);
			delete tgtProg;
		}
		break;
	case ACTIONTYPE_SPEEDDOWN:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getMaxMoves() < selectedAction_->power_)
			tgtProg->setMaxMoves(0);
		else
			tgtProg->setMaxMoves(tgtProg->getMaxMoves() - selectedAction_->power_);

		if (tgtProg->getMoves() > tgtProg->getMaxMoves())
			tgtProg->setMoves(tgtProg->getMaxMoves());
		break;
	case ACTIONTYPE_SPEEDUP:
		if (tgtProg == NULL)
			return;

		tgtProg->setMaxMoves(tgtProg->getMaxMoves() + selectedAction_->power_);
		tgtProg->setMoves(tgtProg->getMoves() + selectedAction_->power_);
		break;
	case ACTIONTYPE_TILEDELETE:
		if (game_->isTiled(pos) && game_->getProgramAt(pos) == NULL)
			game_->setTileAt(pos, TILE_NONE);
		break;
	case ACTIONTYPE_TILEPLACE:
		if (!game_->isTiled(pos))
			game_->setTileAt(pos, TILE_PLAIN);
		break;
	case ACTIONTYPE_MAXHEALTHDOWN:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getMaxHealth() < selectedAction_->power_ - 1)
			tgtProg->setMaxHealth(0);
		else
			tgtProg->setMaxHealth(tgtProg->getMaxHealth() - selectedAction_->power_);
		break;
	case ACTIONTYPE_MAXHEALTHUP:
		if (tgtProg == NULL)
			return;

		tgtProg->setMaxHealth(tgtProg->getMaxHealth() + selectedAction_->power_);
		break;
	case ACTIONTYPE_HEAL:
		printf("CLIENT ERR: action type HEAL not implemented yet\n");
	default:
		break;
	}

	Message m;
	m.type = MSGTYPE_ACTION;
	m.clientID = 0;
	m.playerID = playerID_;
	m.programID = selectedProgram_->getProgramID();
	m.actionID = selectedProgram_->getActions()->getIndexOf(selectedAction_);
	printf("SERVER: actionID %i\n", m.actionID);
	m.pos = pos;
	_server->sendMessageToAllClients(m);

	selectedProgram_->setActionsLeft(selectedProgram_->getActionsLeft() - 1);
	setSelectedAction(NULL);
	setSelectedProgram(selectedProgram_);

	game_->checkForWinCondition();
}

int Player::getPlayerID() {
	return playerID_;
}

void Player::setPlayerID(int playerID) {
	playerID_ = playerID;
}

SDL_Color Player::getColor() {
	return color_;
}

Program* Player::getProgramByID(int progID) {
	Iterator<Program*> it = progsOwned_->getIterator();
	while (it.hasNext()) {
		Program* curr = it.next();
		if (curr->getProgramID() == progID)
			return curr;
	}

	return NULL;
}

void Player::setDoneTurn(bool b) {
	doneTurn_ = b;
}

Game* Player::getGame() {
	return game_;
}

AICore* Player::getMind() {
	return brain_;
}

void Player::setMind(AICore* mind) {
	brain_ = mind;
}