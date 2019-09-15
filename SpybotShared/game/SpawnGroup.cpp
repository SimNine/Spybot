#include "Standard.h"
#include "SpawnGroup.h"

#include "Global.h"
#include "MiscUtil.h"
#include "Message.h"
#include "Server.h"

SpawnGroup::SpawnGroup() {
	tiles_ = new LinkedList<Coord*>();
	groupID_ = randInt();
	playerID_ = -1;
}

SpawnGroup::~SpawnGroup() {
	while (tiles_->getLength() > 0)
		delete tiles_->poll();
	delete tiles_;
}

int SpawnGroup::getGroupID() {
	return groupID_;
}

bool SpawnGroup::containsTile(Coord pos) {
	Iterator<Coord*> it = tiles_->getIterator();
	while (it.hasNext()) {
		Coord* curr = it.next();
		if (curr->x == pos.x && curr->y == pos.y) {
			return true;
		}
	}

	return false;
}

void SpawnGroup::addTile(Coord pos) {
	if (containsTile(pos)) {
		log("SERVER ERR: tried to add tile to spawngroup " + to_string(groupID_) + " which already contains it\n");
		return;
	}

	tiles_->addFirst(new Coord(pos));

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_SPAWNGROUPADDTILE;
	m.num = groupID_;
	m.pos = pos;
	_server->sendMessageToAllClients(m);
}

void SpawnGroup::removeTile(Coord pos) {
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
		log("SERVER ERR: tried to remove tile from spawngroup " + to_string(groupID_) + " which doesn't contain it\n");
		return;
	}

	tiles_->remove(toRemove);
	delete toRemove;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_SPAWNGROUPREMOVETILE;
	m.num = groupID_;
	m.pos = pos;
	_server->sendMessageToAllClients(m);
}

LinkedList<Coord*>* SpawnGroup::getTiles() {
	return tiles_;
}

int SpawnGroup::getPlayerID() {
	return playerID_;
}

void SpawnGroup::setPlayerID(int playerID) {
	playerID_ = playerID;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_SPAWNGROUPSETPLAYER;
	m.num = groupID_;
	m.playerID = playerID;
	_server->sendMessageToAllClients(m);
}