#include "Standard.h"
#include "AIModerate.h"

#include "Player.h"

AIModerate::AIModerate(Player* owner)
	: AICore(owner) {
	//ctor
}

AIModerate::~AIModerate() {
	//dtor
}

bool AIModerate::actStep() {
	// get the next usable program
	Program* myProg = getNextUsableProgram();
	if (myProg == NULL) return false;

	// select this program and compute its distances
	owner_->setSelectedProgram(myProg);

	// get the nearest enemy program to this program

	// TODO
	return false;
}
