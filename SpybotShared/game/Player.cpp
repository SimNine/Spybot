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
#include "Team.h"

Player::Player(Game* g, int teamID) {
	game_ = g;
	team_ = teamID;
	playerID_ = randInt();
	doneTurn_ = false;
	selectedTile_ = NULLCOORD;
	selectedProgram_ = NULL;
	progsOwned_ = new LinkedList<Program*>();
	color_ = { (Uint8)rand(), (Uint8)rand(), (Uint8)rand(), (Uint8)rand() };
	brain_ = NULL;
}

Player::~Player() {
	while (progsOwned_->getLength() > 0)
		delete progsOwned_->poll();
	delete progsOwned_;

	if (brain_ != NULL)
		delete brain_;
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
	m.programID = (p == NULL) ? -1 : p->getProgramID();
	_server->sendMessageToAllClients(m);
}

void Player::moveSelectedProgram(Coord pos) {
	// check for validity
	if (!game_->isTiled(pos))
		return;

	// if the given space is adjacent to the selected program,
	// and the selected program has moves left
	if (selectedProgDist_[pos.x][pos.y] == 1 && selectedProgram_->getMoves() > 0) {
		// move the program
		selectedProgram_->moveTo(pos);
		calculateProgramDist(selectedProgram_);

		// re-select the program and the tile
		setSelectedProgram(selectedProgram_);
		setSelectedTile(pos);

		// send the move sound to the client
		Message msg;
		msg.type = MSGTYPE_SOUND;
		msg.soundType = MSGSOUNDNAME_MOVE;
		msg.num = 0;
		_server->sendMessageToAllClients(msg);

		// if there is a credit here, pick it up
		if (game_->getItemAt(pos) == ITEM_CREDIT && brain_ == NULL) {
			game_->setItemAt(pos, ITEM_NONE);

			msg.soundType = MSGSOUNDNAME_PICKUPCREDIT;
			_server->sendMessageToAllClients(msg);

			Pipe* recievingClient = NULL;
			Iterator<Pipe*> it = _server->getClientList()->getIterator();
			while (it.hasNext()) {
				Pipe* curr = it.next();
				if (curr->getPlayer() == playerID_) {
					Message msg;
					msg.type = MSGTYPE_CREDITPICKUP;
					msg.pos = pos;
					msg.num = rand() % 400;
					_server->getUserByName(curr->getUser())->numCredits_ += msg.num;
					_server->sendMessageToClient(msg, curr->getClientID());
					_server->saveUsers();
					break;
				}
			}
		}
	}
}

void Player::moveSelectedProgramBy(Coord disp) {
	Coord n = selectedProgram_->getHead() + disp;
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
	if (selectedProgDist_[selectedProgram_->getHead().x + disp.x][selectedProgram_->getHead().y + disp.y] >= 1)
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

	Coord h = p->getHead();
	Coord f = p->getHead();
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

	Coord h = p->getHead();
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
		selectedTile_ = NULLCOORD;

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

	setSelectedTile(NULLCOORD);
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

	Coord center = selectedProgram_->getHead();
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
	if (selectedAction_ == NULL || 
		game_->isOOB(pos) || 
		selectedProgram_->getActionsLeft() <= 0 ||
		selectedProgram_->getHealth() < selectedAction_->minSize_)
		return;

	Program* tgtProg = game_->getProgramAt(pos);

	int tileFadeDelay = 200;

	switch (selectedAction_->type_) {
	case ACTIONTYPE_DAMAGE:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() == team_)
			return;

		{
			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_ATTACK;
			m.pos = tgtProg->getHead();
			m.num = selectedAction_->power_;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_DAMAGE;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}

		for (int i = 0; i < selectedAction_->power_; i++) {
			Coord tail = tgtProg->getTail();
			if (tail == NULLCOORD)
				break;

			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_PROGRAMTILEDESTROY;
			m.pos = tail;
			m.num = (i + 1) * tileFadeDelay;
			_server->sendMessageToAllClients(m);

			tgtProg->removeTile(tail);
		}

		if (tgtProg->getHealth() <= 0) {
			game_->removeProgram(tgtProg->getProgramID(), tgtProg->getOwner()->getPlayerID(), tgtProg->getOwner()->getTeam());
		}
		break;
	case ACTIONTYPE_SPEEDDOWN:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() == team_)
			return;

		tgtProg->setMaxMoves(tgtProg->getMaxMoves() - selectedAction_->power_);

		{
			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_ATTACK;
			m.pos = tgtProg->getHead();
			m.num = selectedAction_->power_;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_SPEEDMOD;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	case ACTIONTYPE_SPEEDUP:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() != team_)
			return;

		tgtProg->setMaxMoves(tgtProg->getMaxMoves() + selectedAction_->power_);
		tgtProg->setMoves(tgtProg->getMoves() + selectedAction_->power_);

		{
			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_ATTACK;
			m.pos = tgtProg->getHead();
			m.num = selectedAction_->power_;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_SPEEDMOD;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	case ACTIONTYPE_TILEDELETE:
		if (!game_->isTiled(pos) || game_->getProgramAt(pos) != NULL)
			return;

		game_->setTileAt(pos, TILE_NONE);

		{
			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_TILEDESTROY;
			m.pos = pos;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_ZERO;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	case ACTIONTYPE_TILEPLACE:
		if (game_->isTiled(pos))
			return;

		game_->setTileAt(pos, TILE_PLAIN);

		{
			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_TILECREATE;
			m.pos = pos;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_ONE;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	case ACTIONTYPE_MAXHEALTHDOWN:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() == team_)
			return;

		tgtProg->setMaxHealth(tgtProg->getMaxHealth() - selectedAction_->power_);
		break;
	case ACTIONTYPE_MAXHEALTHUP:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() != team_)
			return;

		tgtProg->setMaxHealth(tgtProg->getMaxHealth() + selectedAction_->power_);
		break;
	case ACTIONTYPE_HEAL:
		log("SERVER ERR: action type HEAL not implemented yet\n");
	case ACTIONTYPE_MAXACTIONSUP:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() != team_)
			return;

		tgtProg->setMaxActions(tgtProg->getMaxActions() + selectedAction_->power_);
		tgtProg->setActionsLeft(tgtProg->getActionsLeft() + selectedAction_->power_);
		log("SELECTEDACTIONPOWER = " + to_string(selectedAction_->power_) + "\n");

		{
			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_ATTACK;
			m.pos = tgtProg->getHead();
			m.num = selectedAction_->power_;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_SPEEDMOD;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	case ACTIONTYPE_MAXACTIONSDOWN:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() == team_)
			return;

		tgtProg->setMaxActions(tgtProg->getMaxActions() - selectedAction_->power_);

		{
			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_ATTACK;
			m.pos = tgtProg->getHead();
			m.num = selectedAction_->power_;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_SPEEDMOD;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	case ACTIONTYPE_TELEPORT:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() != team_)
			return;

		{
			// identify all unoccupied tiles
			LinkedList<Coord*>* openTiles = new LinkedList<Coord*>();
			for (int x = game_->getLeftBound(); x < game_->getRightBound(); x++) {
				for (int y = game_->getTopBound(); y < game_->getBottomBound(); y++) {
					if (game_->getTileAt({ x, y }) != TILE_NONE && game_->getProgramAt({ x, y }) == NULL) {
						openTiles->addFirst(new Coord{ x, y });
					}
				}
			}

			// pick one
			int location = rand() % openTiles->getLength();
			Coord pos = *openTiles->getObjectAt(location);

			// destroy coord list
			while (openTiles->getLength() > 0)
				delete openTiles->poll();
			delete openTiles;

			// if this program is at max health (or max health minus one)
			if (tgtProg->getHealth() >= tgtProg->getMaxHealth() - 1) {
				Coord tail = tgtProg->getTail();
				if (tail == NULLCOORD)
					break;

				Message m;
				m.type = MSGTYPE_INFO;
				m.infoType = MSGINFOTYPE_ANIM;
				m.animType = ANIMTYPE_PROGRAMTILEDESTROY;
				m.pos = tail;
				m.num = tileFadeDelay;
				_server->sendMessageToAllClients(m);

				tgtProg->removeTile(tail);
			}

			tgtProg->addHead(pos);

			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_ACTION_ATTACK;
			m.pos = pos;
			m.num = 20;
			_server->sendMessageToAllClients(m);

			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_SPEEDMOD;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	case ACTIONTYPE_TRANSMIT:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() != team_)
			return;

		{
			// identify all other transmitters on this team
			LinkedList<Program*>* transmitterList = new LinkedList<Program*>();
			Iterator<Player*> itPlayers = game_->getTeamByID(team_)->getAllPlayers()->getIterator();
			while (itPlayers.hasNext()) {
				Player* currPlayer = itPlayers.next();
				Iterator<Program*> itProgs = currPlayer->getProgList()->getIterator();
				while (itProgs.hasNext()) {
					Program* currProg = itProgs.next();
					if (currProg != selectedProgram_ && currProg->getType() == PROGRAM_TRANSMITTER)
						transmitterList->addFirst(currProg);
				}
			}

			// identify all unoccupied tiles adjacent to 
			LinkedList<Coord*>* openTiles = new LinkedList<Coord*>();
			Iterator<Program*> itTransmitters = transmitterList->getIterator();
			while (itTransmitters.hasNext()) {
				Coord c = itTransmitters.next()->getHead();
				Coord left = c + Coord{ -1, 0 };
				Coord right = c + Coord{ 1, 0 };
				Coord top = c + Coord{ 0, -1 };
				Coord bottom = c + Coord{ 0, 1 };

				if (game_->getTileAt(left) != TILE_NONE && game_->getProgramAt(left) == NULL)
					openTiles->addFirst(new Coord(left));
				if (game_->getTileAt(right) != TILE_NONE && game_->getProgramAt(right) == NULL)
					openTiles->addFirst(new Coord(right));
				if (game_->getTileAt(top) != TILE_NONE && game_->getProgramAt(top) == NULL)
					openTiles->addFirst(new Coord(top));
				if (game_->getTileAt(bottom) != TILE_NONE && game_->getProgramAt(bottom) == NULL)
					openTiles->addFirst(new Coord(bottom));
			}

			// pick one and use it
			if (openTiles->getLength() > 0) {
				int location = rand() % openTiles->getLength();
				Coord pos = *openTiles->getObjectAt(location);
				log("pos: " + to_string(pos.x) + "," + to_string(pos.y));

				// if this program is at max health (or max health minus one)
				if (tgtProg->getHealth() >= tgtProg->getMaxHealth() - 1) {
					Coord tail = tgtProg->getTail();
					if (tail == NULLCOORD)
						break;

					Message m;
					m.type = MSGTYPE_INFO;
					m.infoType = MSGINFOTYPE_ANIM;
					m.animType = ANIMTYPE_PROGRAMTILEDESTROY;
					m.pos = tail;
					m.num = tileFadeDelay;
					_server->sendMessageToAllClients(m);

					tgtProg->removeTile(tail);
				}

				// place the new head of the program
				tgtProg->addHead(pos);

				// show an animation at the tile the head was placed
				Message m;
				m.type = MSGTYPE_INFO;
				m.infoType = MSGINFOTYPE_ANIM;
				m.animType = ANIMTYPE_ACTION_ATTACK;
				m.pos = pos;
				m.num = 20;
				_server->sendMessageToAllClients(m);

				// play a sound
				m.type = MSGTYPE_SOUND;
				m.soundType = MSGSOUNDNAME_SPEEDMOD;
				m.num = 0;
				_server->sendMessageToAllClients(m);
			}

			// destroy transmitter list
			while (transmitterList->getLength() > 0)
				transmitterList->poll();
			delete transmitterList;

			// destroy coord list
			while (openTiles->getLength() > 0)
				delete openTiles->poll();
			delete openTiles;
		}
		break;
	case ACTIONTYPE_FRAGMENT:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getTeam() == team_)
			return;

		{
			// identify all unoccupied tiles
			LinkedList<Coord*>* openTiles = new LinkedList<Coord*>();
			for (int x = game_->getLeftBound(); x < game_->getRightBound(); x++) {
				for (int y = game_->getTopBound(); y < game_->getBottomBound(); y++) {
					if (game_->getTileAt({ x, y }) != TILE_NONE && game_->getProgramAt({ x, y }) == NULL) {
						openTiles->addFirst(new Coord{ x, y });
					}
				}
			}

			// for each tile in the targeted program
			for (int i = 0; i < tgtProg->getHealth(); i++) {
				// pick a random unoccupied tile
				int location = rand() % openTiles->getLength();
				Coord* pos = openTiles->removeObjectAt(location);

				// get the targeted program's tail
				Coord tail = tgtProg->getTail();

				// send tilefade anim
				Message m;
				m.type = MSGTYPE_INFO;
				m.infoType = MSGINFOTYPE_ANIM;
				m.animType = ANIMTYPE_PROGRAMTILEDESTROY;
				m.pos = tail;
				m.num = tileFadeDelay;
				_server->sendMessageToAllClients(m);

				// send tile appear anim
				m.type = MSGTYPE_INFO;
				m.infoType = MSGINFOTYPE_ANIM;
				m.animType = ANIMTYPE_ACTION_ATTACK;
				m.pos = *pos;
				m.num = 5;
				_server->sendMessageToAllClients(m);

				// move the program
				tgtProg->removeTile(tail);
				tgtProg->addHead(*pos);
				delete pos;
			}

			// destroy coord list
			while (openTiles->getLength() > 0)
				delete openTiles->poll();
			delete openTiles;

			// send sound
			Message m;
			m.type = MSGTYPE_SOUND;
			m.soundType = MSGSOUNDNAME_SPEEDMOD;
			m.num = 0;
			_server->sendMessageToAllClients(m);
		}
		break;
	default:
		break;
	}

	// if this action has a cost to the user, make it happen
	if (selectedAction_->sizeCost_ != 0) {
		int numCost;
		if (selectedAction_->sizeCost_ == -1)
			numCost = selectedProgram_->getHealth();
		else
			numCost = selectedAction_->sizeCost_;

		// for each tile that is spent
		for (int i = 0; i < numCost; i++) {
			Coord tail = selectedProgram_->getTail();
			if (tail == NULLCOORD)
				break;

			Message m;
			m.type = MSGTYPE_INFO;
			m.infoType = MSGINFOTYPE_ANIM;
			m.animType = ANIMTYPE_PROGRAMTILEDESTROY;
			m.pos = tail;
			m.num = (i + 1) * tileFadeDelay;
			_server->sendMessageToAllClients(m);

			selectedProgram_->removeTile(tail);
		}

		if (selectedProgram_->getHealth() <= 0) {
			game_->removeProgram(selectedProgram_->getProgramID(), selectedProgram_->getOwner()->getPlayerID(), selectedProgram_->getOwner()->getTeam());
			setSelectedAction(NULL);
			setSelectedProgram(NULL);
			game_->checkForWinCondition();
			return;
		}
	}

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

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PLAYERSETMIND;
	m.playerID = playerID_;
	m.num = (mind == NULL ? 0 : 1);
	_server->sendMessageToAllClients(m);
}