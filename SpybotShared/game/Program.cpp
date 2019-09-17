#include "Standard.h"
#include "Program.h"

#include "Global.h"
#include "Player.h"
#include "ProgramAction.h"
#include "MiscUtil.h"
#include "Message.h"
#include "Server.h"
#include "Game.h"
#include "ProgramPreset.h"

Program::Program(PROGRAM type) {
	this->type_ = type;
	programID_ = randInt();
	color_[0] = rand() % 255;
	color_[1] = rand() % 255;
	color_[2] = rand() % 255;
	actionList_ = new LinkedList<ProgramAction*>();
	tiles_ = new LinkedList<Coord*>();

	if (this->type_ == PROGRAM_CUSTOM)
		return;

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

Program::~Program() {
	while (tiles_->getLength() > 0)
		delete tiles_->poll();
	delete tiles_;

	while (actionList_->getLength() > 0)
		delete actionList_->poll();
	delete actionList_;

	if (_debug >= DEBUG_NORMAL)
		log("SERVER: program '" + name_ + "' deleted\n");
}

int Program::getColor(int n) {
	if (n < 0 || n > 2)
		return 0;

	return color_[n];
}

void Program::setColor(int r, int g, int b) {
	color_[0] = r;
	color_[1] = g;
	color_[2] = b;
}

int Program::getTeam() {
	return owner_->getTeam();
}

int Program::getHealth() {
	return tiles_->getLength();
}

int Program::getMaxHealth() {
	return maxHealth_;
}

PROGRAM Program::getType() {
	return type_;
}

int Program::getMoves() {
	return moves_;
}

int Program::getMaxMoves() {
	return maxMoves_;
}

void Program::setType(PROGRAM i) {
	type_ = i;
}

void Program::setMaxHealth(int i) {
	int newMax = (i < 0) ? 0 : i;

	while (newMax < tiles_->getLength()) {
		Coord tail = getTail();
		if (tail == NULLCOORD)
			break;
		removeTile(tail);
	}

	maxHealth_ = newMax;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMCHANGEMAXHEALTH;
	m.num = newMax;
	m.teamID = owner_->getTeam();
	m.playerID = owner_->getPlayerID();
	m.programID = programID_;
	_server->sendMessageToAllClients(m);
}

void Program::setMoves(int i) {
	int newMoves = (i < 0) ? 0 : i;

	moves_ = newMoves;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMCHANGENUMMOVES;
	m.num = newMoves;
	m.teamID = owner_->getTeam();
	m.playerID = owner_->getPlayerID();
	m.programID = programID_;
	_server->sendMessageToAllClients(m);
}

void Program::setMaxMoves(int i) {
	int newMaxMoves = (i < 0) ? 0 : i;

	if (moves_ > newMaxMoves)
		setMoves(newMaxMoves);

	maxMoves_ = i;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMCHANGEMAXMOVES;
	m.num = newMaxMoves;
	m.teamID = owner_->getTeam();
	m.playerID = owner_->getPlayerID();
	m.programID = programID_;
	_server->sendMessageToAllClients(m);
}

std::string Program::getName() {
	return name_;
}

void Program::setName(std::string n) {
	name_ = n;
}

void Program::addAction(ACTION p) {
	actionList_->addLast(new ProgramAction(p));
}

void Program::endTurn() {
	actionsLeft_ = maxActions_;
	moves_ = maxMoves_;
}

LinkedList<ProgramAction*>* Program::getActions() {
	return actionList_;
}

Coord Program::getHead() {
	Coord* ret = tiles_->getFirst();
	if (ret == NULL)
		return NULLCOORD;
	else
		return *ret;
}

Coord Program::getTail() {
	Coord* ret = tiles_->getLast();
	if (ret == NULL)
		return NULLCOORD;
	else
		return *ret;
}

void Program::moveTo(Coord pos) {
	// decrement number of moves left
	setMoves(getMoves() - 1);

	// add the new head
	addHead(pos);

	// if this program is above max health, disown the last tile
	if (tiles_->getLength() > maxHealth_)
		removeTile(getTail());
}

void Program::addHead(Coord pos) {
	// remove this tile if it's already owned by the program
	removeTile(pos);

	// add the tile to the front of the queue
	tiles_->addFirst(new Coord(pos));
	owner_->getGame()->setProgramAt(pos, this);

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMADDHEAD;
	m.teamID = owner_->getTeam();
	m.playerID = owner_->getPlayerID();
	m.programID = programID_;
	m.pos = pos;
	_server->sendMessageToAllClients(m);
}

void Program::addTail(Coord pos) {
	// remove this tile if it's already owned by the program
	removeTile(pos);

	// add the tile to the end of the queue
	tiles_->addLast(new Coord(pos));
	owner_->getGame()->setProgramAt(pos, this);

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMADDTAIL;
	m.teamID = owner_->getTeam();
	m.playerID = owner_->getPlayerID();
	m.programID = programID_;
	m.pos = pos;
	_server->sendMessageToAllClients(m);
}

void Program::removeTile(Coord pos) {
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
		delete toRemove;
		owner_->getGame()->setProgramAt(pos, NULL);

		Message m;
		m.type = MSGTYPE_INFO;
		m.infoType = MSGINFOTYPE_PROGRAMREMOVETILE;
		m.teamID = owner_->getTeam();
		m.playerID = owner_->getPlayerID();
		m.programID = programID_;
		m.pos = pos;
		_server->sendMessageToAllClients(m);
	}
}

int Program::getActionsLeft() {
	return actionsLeft_;
}

void Program::setActionsLeft(int i) {
	int newActionsLeft = (i < 0) ? 0 : i;

	actionsLeft_ = newActionsLeft;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMCHANGENUMACTIONS;
	m.num = newActionsLeft;
	m.teamID = owner_->getTeam();
	m.playerID = owner_->getPlayerID();
	m.programID = programID_;
	_server->sendMessageToAllClients(m);
}

int Program::getMaxActions() {
	return maxActions_;
}

void Program::setMaxActions(int i) {
	int newMaxActions = (i < 0) ? 0 : i;

	if (actionsLeft_ > newMaxActions)
		setActionsLeft(newMaxActions);

	maxActions_ = newMaxActions;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMCHANGEMAXACTIONS;
	m.num = newMaxActions;
	m.teamID = owner_->getTeam();
	m.playerID = owner_->getPlayerID();
	m.programID = programID_;
	_server->sendMessageToAllClients(m);
}

bool Program::isDone() {
	if (moves_ == 0 && actionsLeft_ == 0)
		return true;
	else
		return false;
}

LinkedList<Coord*>* Program::getTiles() {
	return tiles_;
}

Player* Program::getOwner() {
	return owner_;
}

void Program::setOwner(Player* p) {
	owner_ = p;
}

int Program::getProgramID() {
	return programID_;
}

ProgramAction* Program::getActionByID(int actionID) {
	Iterator<ProgramAction*> it = actionList_->getIterator();
	while (it.hasNext()) {
		ProgramAction* curr = it.next();
		if (curr->actionID_ == actionID)
			return curr;
	}

	return NULL;
}