#include "Standard.h"
#include "ClientMirror.h"

ClientMirror::ClientMirror() {
	clientID_ = -1;
	player_ = NULL;
	name_ = "null";
	for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
		ownedProgs_[i] = 0;
		inPlayProgs_[i] = 0;
	}
}

ClientMirror::~ClientMirror() {
	//dtor
}