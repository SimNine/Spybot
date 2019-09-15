#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;

class TeamMirror {
public:
	TeamMirror(int teamID);
	virtual ~TeamMirror();

	LinkedList<PlayerMirror*>* getAllPlayers();
	PlayerMirror* getPlayerByID(int playerID);

	int getTeamID();
protected:

private:
	LinkedList<PlayerMirror*>* players_;
	int teamID_;
};