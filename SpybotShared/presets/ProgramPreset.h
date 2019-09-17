#pragma once

#include "Standard.h"

struct ProgramPreset {
	std::string name_;
	std::string desc_;
	int maxMoves_;
	int maxHealth_;
	int maxActions_;
	int cost_;
	ACTION action1_;
	ACTION action2_;
	ACTION action3_;
};

ProgramPreset getProgramPreset(PROGRAM p);