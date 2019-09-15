#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;

class ClientMirror {
public:
	ClientMirror();
	virtual ~ClientMirror();

	int clientID_;
	PlayerMirror* player_;
	std::string name_;

	int ownedProgs_[PROGRAM_NUM_PROGTYPES];
	int inPlayProgs_[PROGRAM_NUM_PROGTYPES];
	int credits_;
protected:

private:
};