#pragma once

#include "Standard.h"

struct ActionPreset {
	std::string name_;
	std::string desc_;

	ACTIONTYPE type_;
	int range_;
	int power_;
	int sizeCost_;
	int numUses_;
	int minSize_;
	int maxSize_;
};

ActionPreset getActionPreset(ACTION a);