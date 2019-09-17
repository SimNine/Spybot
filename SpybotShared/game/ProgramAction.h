#pragma once

#include "Standard.h"

class ProgramAction {
public:
	ProgramAction();
	ProgramAction(ACTION);
	virtual ~ProgramAction();

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