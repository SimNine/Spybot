#pragma once

#include "Standard.h"
#include "Message.h"

class GameConfig {
public:
	GameConfig();
	virtual ~GameConfig();

	GAMEMODE gameMode_;
	std::string campaign_;
	std::string level_;
protected:
private:
};