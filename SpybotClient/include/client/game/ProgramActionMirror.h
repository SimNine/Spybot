#pragma once

#include "Standard.h"

class ProgramActionMirror {
public:
	ProgramActionMirror();
	ProgramActionMirror(MOVEPRESET);
	ProgramActionMirror(ACTIONTYPE, std::string, std::string, int, int, int);
	virtual ~ProgramActionMirror();

	ACTIONTYPE type_;
	std::string name_;
	std::string description_;
	int range_;
	int power_;
	int requiredSize_;
	bool hitsFriendlies_, hitsEnemies_;
	int actionID_;
protected:
private:
	void setMove(ACTIONTYPE, std::string, std::string, int, int, int);
};