#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;

class Team {
public:
	Team();
	virtual ~Team();

	LinkedList<Player*>* getAllPlayers();
	Player* getPlayerByID(int playerID);

	void setTeamID(int teamID);
	int getTeamID();

	void setDefaultAI(bool b);
	bool isDefaultAI();
	void setDefaultHuman(bool b);
	bool isDefaultHuman();
protected:

private:
	LinkedList<Player*>* players_;
	int teamID_;

	bool isDefaultAI_;
	bool isDefaultHuman_;
};