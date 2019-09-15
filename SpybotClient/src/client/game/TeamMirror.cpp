#include "Standard.h"
#include "TeamMirror.h"

#include "PlayerMirror.h"

TeamMirror::TeamMirror(int teamNum) {
	players_ = new LinkedList<PlayerMirror*>();
	teamNum_ = teamNum;
}

TeamMirror::~TeamMirror() {
	while (players_->getLength() > 0)
		delete players_->poll();
	delete players_;
}

LinkedList<PlayerMirror*>* TeamMirror::getAllPlayers() {
	return players_;
}

PlayerMirror* TeamMirror::getPlayerByID(int playerID) {
	Iterator<PlayerMirror*> it = players_->getIterator();
	while (it.hasNext()) {
		PlayerMirror* curr = it.next();
		if (curr->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
}

int TeamMirror::getTeamNum() {
	return teamNum_;
}