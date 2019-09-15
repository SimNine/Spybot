#pragma once

#include "Standard.h"

class ProgramAction {
public:
	ProgramAction(MOVEPRESET);
	ProgramAction(ACTIONTYPE, std::string, std::string, int, int, int);
	virtual ~ProgramAction();

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