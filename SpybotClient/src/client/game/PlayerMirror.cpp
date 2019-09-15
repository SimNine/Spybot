#include "Standard.h"
#include "PlayerMirror.h"

#include "Global.h"
#include "MiscUtil.h"
#include "GameMirror.h"
#include "ProgramMirror.h"
#include "ProgramActionMirror.h"
#include "Client.h"
#include "ProgramDisplayContainer.h"

#include "GameOverlay.h"
#include "AnimationTileFade.h"
#include "AnimationAttack.h"

PlayerMirror::PlayerMirror(GameMirror* g, int playerID, int teamID) {
	game_ = g;
	playerID_ = playerID;
	teamID_ = teamID;
	selectedTile_ = NULLCOORD;
	selectedProgram_ = NULL;
	progsOwned_ = new LinkedList<ProgramMirror*>();
	color_ = { (Uint8)rand(), (Uint8)rand(), (Uint8)rand(), (Uint8)rand() };
}

PlayerMirror::~PlayerMirror() {
	log("CLIENT: player " + to_string(playerID_) + " deleted\n");

	while (progsOwned_->getLength() > 0)
		delete progsOwned_->poll();
	delete progsOwned_;
}

void PlayerMirror::setSelectedProgram(ProgramMirror* p) {
	if (p == NULL || p->getTeamID() != teamID_)
		calculateProgramDist(NULL);
	else
		calculateProgramDist(p);

	setSelectedAction(NULL);
	selectedProgram_ = p;

	if (_client->getPlayer() == this) {
		_gameOverlay->getProgramDisplayContainer()->setCurrProg(p);
	}
}

bool PlayerMirror::canSelectedProgramMoveTo(Coord pos) {
	// in case something breaks
	if (selectedProgram_ == NULL)
		return false;

	// in case this isn't your program
	if (selectedProgram_->getOwner() != this)
		return false;

	// otherwise...
	if (selectedProgDist_[pos.x][pos.y] == 1)
		return true;
	else
		return false;
}

ProgramMirror* PlayerMirror::getSelectedProgram() {
	return selectedProgram_;
}

int PlayerMirror::getSelectedProgramDist(Coord pos) {
	return selectedProgDist_[pos.x][pos.y];
}

int PlayerMirror::getSelectedProgramDistAll(Coord pos) {
	return selectedProgDistAll_[pos.x][pos.y];
}

void PlayerMirror::calculateProgramDist(ProgramMirror* p) {
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

void PlayerMirror::addProgram(ProgramMirror* p) {
	if (progsOwned_->contains(p))
		return;

	progsOwned_->addLast(p);
	p->setOwner(this);
}

void PlayerMirror::setSelectedTile(Coord pos) {
	selectedTile_ = pos;
}

Coord PlayerMirror::getSelectedTile() {
	return selectedTile_;
}

void PlayerMirror::endTurn() {
	Iterator<ProgramMirror*> it = progsOwned_->getIterator();
	while (it.hasNext())
		it.next()->endTurn();

	setSelectedTile(NULLCOORD);
	setSelectedProgram(NULL);
}

LinkedList<ProgramMirror*>* PlayerMirror::getProgList() {
	return progsOwned_;
}

int PlayerMirror::getTeamID() {
	return teamID_;
}

void PlayerMirror::setSelectedAction(ProgramActionMirror* pa) {
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
		for (int y = -rngLeft; y <= rngLeft; y++) {
			selectedActionDistAll_[center.x + x][center.y + y] = dist(center, center + Coord{ x, y });
			if (game_->isTiled(center + Coord{ x, y }))
				selectedActionDist_[center.x + x][center.y + y] = dist(center, center + Coord{ x, y });
		}
	}
	selectedActionDist_[center.x][center.y] = 0;
	selectedActionDistAll_[center.x][center.y] = 0;

	selectedAction_ = pa;
}

ProgramActionMirror* PlayerMirror::getSelectedAction() {
	return selectedAction_;
}

int PlayerMirror::getSelectedActionDist(Coord pos) {
	return selectedActionDist_[pos.x][pos.y];
}

int PlayerMirror::getSelectedActionDistAll(Coord pos) {
	return selectedActionDistAll_[pos.x][pos.y];
}

int PlayerMirror::getPlayerID() {
	return playerID_;
}

SDL_Color PlayerMirror::getColor() {
	return color_;
}

ProgramMirror* PlayerMirror::getProgramByID(int progID) {
	Iterator<ProgramMirror*> it = progsOwned_->getIterator();
	while (it.hasNext()) {
		ProgramMirror* curr = it.next();
		if (curr->getProgramID() == progID)
			return curr;
	}

	return NULL;
}

GameMirror* PlayerMirror::getGame() {
	return game_;
}