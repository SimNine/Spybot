#include "Standard.h"
#include "ProgramAction.h"

#include "Global.h"
#include "ActionPreset.h"

ProgramAction::ProgramAction() {
	this->name_ = "";
	this->desc_ = "";

	this->type_ = ACTIONTYPE_NONE;
	this->range_ = 0;
	this->power_ = 0;
	this->sizeCost_ = 0;
	this->numUses_ = 0;
	this->minSize_ = 0;
	this->maxSize_ = -1;

	this->actionID_ = -1;
}

ProgramAction::ProgramAction(ACTION a) {
	this->name_ = "";
	this->desc_ = "";

	this->type_ = ACTIONTYPE_NONE;
	this->range_ = 0;
	this->power_ = 0;
	this->sizeCost_ = 0;
	this->numUses_ = 0;
	this->minSize_ = 0;
	this->maxSize_ = -1;

	this->actionID_ = -1;

	if (a == ACTION_NONE &&
		a == ACTION_CUSTOM &&
		a == ACTION_NUM_ACTIONPRESETS)
		return;

	ActionPreset preset = getActionPreset(a);

	this->name_ = preset.name_;
	this->desc_ = preset.desc_;

	this->type_ = preset.type_;
	this->range_ = preset.range_;
	this->power_ = preset.power_;
	this->sizeCost_ = preset.sizeCost_;
	this->numUses_ = preset.numUses_;
	this->minSize_ = preset.minSize_;
	this->maxSize_ = preset.maxSize_;
}

ProgramAction::~ProgramAction() {
	if (_debug >= DEBUG_NORMAL)
		log("SERVER: Action '" + name_ + "' deleted\n");
}
