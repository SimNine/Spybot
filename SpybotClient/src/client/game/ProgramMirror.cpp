#include "Standard.h"
#include "ProgramMirror.h"

#include "Global.h"
#include "PlayerMirror.h"
#include "ProgramActionMirror.h"
#include "GameMirror.h"
#include "ProgramPreset.h"

ProgramMirror::ProgramMirror(PROGRAM type, int programID) {
	this->type_ = type;
	programID_ = programID;
	color_[0] = rand() % 255;
	color_[1] = rand() % 255;
	color_[2] = rand() % 255;
	actionList_ = new LinkedList<ProgramActionMirror*>();
	tiles_ = new LinkedList<Coord*>();

	if (this->type_ == PROGRAM_CUSTOM) {
		return;
	}

	ProgramPreset preset = getProgramPreset(type);

	name_ = preset.name_;
	description_ = preset.desc_;
	maxActions_ = preset.maxActions_;
	maxMoves_ = preset.maxMoves_;
	maxHealth_ = preset.maxHealth_;
	cost_ = preset.cost_;
	if (preset.action1_ != ACTION_NONE)
		addAction(preset.action1_);
	if (preset.action2_ != ACTION_NONE)
		addAction(preset.action2_);
	if (preset.action3_ != ACTION_NONE)
		addAction(preset.action3_);

	moves_ = maxMoves_;
	actionsLeft_ = maxActions_;
}

ProgramMirror::~ProgramMirror() {
	log("CLIENT: Program '" + name_ + "' deleted\n");

	while (tiles_->getLength() > 0)
		delete tiles_->poll();
	delete tiles_;

	while (actionList_->getLength() > 0)
		delete actionList_->poll();
	delete actionList_;
}

int ProgramMirror::getColor(int n) {
	if (n < 0 || n > 2) {
		return 0;
	}

	return color_[n];
}

void ProgramMirror::setColor(int r, int g, int b) {
	color_[0] = r;
	color_[1] = g;
	color_[2] = b;
}

int ProgramMirror::getTeamID() {
	return owner_->getTeamID();
}

int ProgramMirror::getHealth() {
	return tiles_->getLength();
}

int ProgramMirror::getMaxHealth() {
	return maxHealth_;
}

PROGRAM ProgramMirror::getType() {
	return type_;
}

int ProgramMirror::getMoves() {
	return moves_;
}

int ProgramMirror::getMaxMoves() {
	return maxMoves_;
}

void ProgramMirror::setType(PROGRAM i) {
	type_ = i;
}

void ProgramMirror::setMaxHealth(int i) {
	maxHealth_ = i;
}

void ProgramMirror::setMoves(int i) {
	if (i < 0)
		moves_ = 0;
	else
		moves_ = i;
}

void ProgramMirror::setMaxMoves(int i) {
	maxMoves_ = i;
}

std::string ProgramMirror::getName() {
	return name_;
}

void ProgramMirror::setName(std::string n) {
	name_ = n;
}

void ProgramMirror::addAction(ACTION p) {
	actionList_->addLast(new ProgramActionMirror(p));
}

void ProgramMirror::endTurn() {
	actionsLeft_ = maxActions_;
	moves_ = maxMoves_;
}

LinkedList<ProgramActionMirror*>* ProgramMirror::getActions() {
	return actionList_;
}

Coord ProgramMirror::getHead() {
	return *tiles_->getFirst();
}

Coord ProgramMirror::getTail() {
	return *tiles_->getLast();
}

void ProgramMirror::addHead(Coord pos) {
	// remove this tile if it's already owned by the program
	removeTile(pos);

	// add the tile to the front of the queue
	tiles_->addFirst(new Coord(pos));
	owner_->getGame()->setProgramAt(pos, this);
}

void ProgramMirror::addTail(Coord pos) {
	// remove this tile if it's already owned by the program
	removeTile(pos);

	// add the tile to the end of the queue
	tiles_->addLast(new Coord(pos));
	owner_->getGame()->setProgramAt(pos, this);
}

void ProgramMirror::removeTile(Coord pos) {
	Coord* toRemove = NULL;
	Iterator<Coord*> it = tiles_->getIterator();
	while (it.hasNext()) {
		Coord* curr = it.next();
		if (curr->x == pos.x && curr->y == pos.y) {
			toRemove = curr;
			break;
		}
	}

	if (toRemove != NULL) {
		tiles_->remove(toRemove);
		owner_->getGame()->setProgramAt(pos, NULL);
	}
}

int ProgramMirror::getActionsLeft() {
	return actionsLeft_;
}

void ProgramMirror::setActionsLeft(int i) {
	actionsLeft_ = i;
}

bool ProgramMirror::isDone() {
	if (moves_ == 0 && actionsLeft_ == 0)
		return true;
	else
		return false;
}

Coord* ProgramMirror::popTail() {
	return tiles_->removeLast();
}

LinkedList<Coord*>* ProgramMirror::getTiles() {
	return tiles_;
}

PlayerMirror* ProgramMirror::getOwner() {
	return owner_;
}

void ProgramMirror::setOwner(PlayerMirror* p) {
	owner_ = p;
}

int ProgramMirror::getProgramID() {
	return programID_;
}

ProgramActionMirror* ProgramMirror::getActionByID(int actionID) {
	Iterator<ProgramActionMirror*> it = actionList_->getIterator();
	while (it.hasNext()) {
		ProgramActionMirror* curr = it.next();
		if (curr->actionID_ == actionID)
			return curr;
	}

	return NULL;
}

int ProgramMirror::getMaxActions() {
	return maxActions_;
}

void ProgramMirror::setMaxActions(int i) {
	maxActions_ = i;
}