#include "Standard.h"
#include "Team.h"

#include "Player.h"
#include "MiscUtil.h"

Team::Team() {
	players_ = new LinkedList<Player*>();
	teamID_ = randInt();

	isDefaultAI_ = false;
	isDefaultHuman_ = false;
}

Team::~Team() {
	while (players_->getLength() > 0)
		delete players_->poll();
	delete players_;
}

LinkedList<Player*>* Team::getAllPlayers() {
	return players_;
}

Player* Team::getPlayerByID(int playerID) {
	Iterator<Player*> it = players_->getIterator();
	while (it.hasNext()) {
		Player* curr = it.next();
		if (curr->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
}

void Team::setTeamID(int teamID) {
	teamID_ = teamID;
}

int Team::getTeamID() {
	return teamID_;
}

void Team::setDefaultHuman(bool b) {
	isDefaultHuman_ = b;
}

bool Team::isDefaultHuman() {
	return isDefaultHuman_;
}

void Team::setDefaultAI(bool b) {
	isDefaultAI_ = b;
}

bool Team::isDefaultAI() {
	return isDefaultAI_;
}