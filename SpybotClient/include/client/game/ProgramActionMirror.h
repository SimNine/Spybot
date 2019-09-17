#pragma once

#include "Standard.h"

class ProgramActionMirror {
public:
	ProgramActionMirror();
	ProgramActionMirror(ACTION);
	ProgramActionMirror(ACTIONTYPE, std::string, std::string, int, int, int);
	virtual ~ProgramActionMirror();

	std::string name_;
	std::string desc_;

	ACTIONTYPE type_;
	int range_;
	int power_;
	int sizeCost_;
	int numUses_;
	int minSize_;
	int maxSize_;

	int actionID_;
protected:
private:
};