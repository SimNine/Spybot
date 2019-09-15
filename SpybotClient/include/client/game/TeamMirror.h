#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;

class Team {
public:
	Team(int teamNum);
	virtual ~Team();

	LinkedList<Player*>* getAllPlayers();
	Player* getPlayerByID(int playerID);
	int getTeamNum();
protected:

private:
	LinkedList<Player*>* players_;
	int teamNum_;
};