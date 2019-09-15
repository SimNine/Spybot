#pragma once

#include "Standard.h"
#include "AICore.h"

class Game;
class Player;

class AIModerate : public AICore {
public:
	AIModerate(Player* owner);
	virtual ~AIModerate();
protected:
	bool actStep();
private:
};