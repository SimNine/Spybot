#include "Standard.h"
#include "ClientMirror.h"

ClientMirror::ClientMirror()
{
	clientID_ = -1;
	player_ = NULL;
	name_ = "null";
	owner_ = false;
}

ClientMirror::~ClientMirror()
{
    //dtor
}