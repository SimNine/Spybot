#pragma once

#include "Standard.h"
#define NUM_CLASSIC_LEVELS 42
#define NUM_NIGHTFALL_LEVELS 40

class User {
public:
	User();
	virtual ~User();

	std::string username_;
	std::string password_;
	int numCredits_;
	bool campaignClassic_[NUM_CLASSIC_LEVELS];
	bool campaignNightfall_[NUM_NIGHTFALL_LEVELS];
	int progsOwned_[PROGRAM_NUM_PROGTYPES];
	int progsInPlay_[PROGRAM_NUM_PROGTYPES];
	int numWins_;
	int numLosses_;
	int numDraws_;
	int numResigns_;
	int numGames_;

	bool loggedIn_;
protected:

private:
};