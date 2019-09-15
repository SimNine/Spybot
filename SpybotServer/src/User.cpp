#include "Standard.h"
#include "User.h"

User::User()
{
	username_ = "";
	password_ = "";
	for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
		progs_[i] = 0;
	numWins_ = 0;
	numLosses_ = 0;
	numDraws_ = 0;
	numResigns_ = 0;
	numGames_ = 0;
	loggedIn_ = false;
}

User::~User()
{
	// dtor
}