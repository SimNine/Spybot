#include "Standard.h"
#include "SpawnGroupMirror.h"

SpawnGroupMirror::SpawnGroupMirror(int groupID) {
	playerID_ = -1;
	groupID_ = groupID;
	tiles_ = new LinkedList<Coord*>();
}

SpawnGroupMirror::~SpawnGroupMirror() {
	//dtor
}

int SpawnGroupMirror::getGroupID() {
	return groupID_;
}

bool SpawnGroupMirror::containsTile(Coord pos) {
	Iterator<Coord*> it = tiles_->getIterator();
	while (it.hasNext()) {
		Coord* curr = it.next();
		if (curr->x == pos.x && curr->y == pos.y) {
			return true;
		}
	}

	return false;
}

void SpawnGroupMirror::addTile(Coord pos) {
	if (containsTile(pos)) {
		log("CLIENT ERR: tried to add tile to spawngroupmirror " + to_string(groupID_) + " which already contains it\n");
		return;
	}

	tiles_->addFirst(new Coord(pos));
}

void SpawnGroupMirror::removeTile(Coord pos) {
	Coord* toRemove = NULL;
	Iterator<Coord*> it = tiles_->getIterator();
	while (it.hasNext()) {
		Coord* curr = it.next();
		if (curr->x == pos.x && curr->y == pos.y) {
			toRemove = curr;
			break;
		}
	}

	if (toRemove == NULL) {
		log("CLIENT ERR: tried to remove tile from spawngroupmirror " + to_string(groupID_) + " which doesn't contain it\n");
		return;
	}

	tiles_->remove(toRemove);
	delete toRemove;
}

LinkedList<Coord*>* SpawnGroupMirror::getTiles() {
	return tiles_;
}

int SpawnGroupMirror::getPlayerID() {
	return playerID_;
}

void SpawnGroupMirror::setPlayerID(int playerID) {
	playerID_ = playerID;
}