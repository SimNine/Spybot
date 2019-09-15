#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;

class TeamMirror {
public:
	TeamMirror(int teamNum);
	virtual ~TeamMirror();

	LinkedList<PlayerMirror*>* getAllPlayers();
	PlayerMirror* getPlayerByID(int playerID);
	int getTeamNum();
protected:

private:
	LinkedList<PlayerMirror*>* players_;
	int teamNum_;
};