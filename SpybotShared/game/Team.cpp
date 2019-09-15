#include "Standard.h"
#include "Team.h"

#include "Player.h"

Team::Team(int teamNum) {
	players_ = new LinkedList<Player*>();
	teamNum_ = teamNum;
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

int Team::getTeamNum() {
	return teamNum_;
}