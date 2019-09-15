#pragma once

#include "Standard.h"
#include "LinkedList.h"

class SpawnGroupMirror {
public:
	SpawnGroupMirror(int groupID);
	virtual ~SpawnGroupMirror();

	int getPlayerID();
	void setPlayerID(int playerID);

	int getGroupID();

	bool containsTile(Coord pos);
	void addTile(Coord pos);
	void removeTile(Coord pos);
	LinkedList<Coord*>* getTiles();
protected:
private:
	int groupID_;
	int playerID_;
	LinkedList<Coord*>* tiles_;
};