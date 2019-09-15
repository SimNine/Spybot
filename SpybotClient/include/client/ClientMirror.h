#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;

class ClientMirror {
public:
	ClientMirror();
	virtual ~ClientMirror();

	int clientID_;
	Player* player_;
	std::string name_;

	int ownedProgs_[PROGRAM_NUM_PROGTYPES];
	int inPlayProgs_[PROGRAM_NUM_PROGTYPES];
	int credits_;
protected:

private:
};