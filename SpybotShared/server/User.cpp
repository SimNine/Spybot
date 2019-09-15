#include "Standard.h"
#include "User.h"

User::User() {
	username_ = "";
	password_ = "";
	numCredits_ = 0;
	for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
		progsOwned_[i] = 0;
		progsInPlay_[i] = 0;
	}
	for (int i = 0; i < NUM_CLASSIC_LEVELS; i++)
		campaignClassic_[i] = false;
	campaignClassic_[0] = true;
	for (int i = 0; i < NUM_NIGHTFALL_LEVELS; i++)
		campaignNightfall_[i] = false;
	campaignNightfall_[0] = true;
	numWins_ = 0;
	numLosses_ = 0;
	numDraws_ = 0;
	numResigns_ = 0;
	numGames_ = 0;
	loggedIn_ = false;
}

User::~User() {
	// dtor
}