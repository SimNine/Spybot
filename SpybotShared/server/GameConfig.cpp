#include "Standard.h"
#include "GameConfig.h"

GameConfig::GameConfig() {
	gameMode_ = GAMEMODE_FFA;
	campaign_ = "multi";
	level_ = "cross";
}

GameConfig::~GameConfig() {
	// dtor
}