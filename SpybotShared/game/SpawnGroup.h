#pragma once

#include "Standard.h"
#include "LinkedList.h"

class SpawnGroup {
public:
	SpawnGroup();
	virtual ~SpawnGroup();

	int getGroupID();
	int getPlayerID();
	void setPlayerID(int playerID);

	bool containsTile(Coord pos);
	void addTile(Coord pos);
	void removeTile(Coord pos);
	LinkedList<Coord*>* getTiles();
private:
	int playerID_;
	int groupID_;
	LinkedList<Coord*>* tiles_;
};