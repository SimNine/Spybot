#include "Standard.h"
#include "ProgramMirror.h"

#include "Global.h"
#include "PlayerMirror.h"
#include "ProgramActionMirror.h"
#include "GameMirror.h"

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

	maxActions_ = 1;
	switch (this->type_) {
	case PROGRAM_NONE:
	case PROGRAM_NUM_PROGTYPES:
		log("CLIENT ERR: trying to instantiate an invalid programmirror type");
		exit(1);
		break;
	case PROGRAM_CUSTOM:
		break;
	case PROGRAM_BALLISTA:
		name_ = "Ballista";
		description_ = "Extreme-range attack program";
		maxMoves_ = 1;
		maxHealth_ = 2;
		cost_ = 3000;
		addAction(MOVEPRESET_FLING);
		break;
	case PROGRAM_BITMAN:
		name_ = "Bit-Man";
		description_ = "Makes sectors of the grid appear or disappear";
		maxMoves_ = 3;
		maxHealth_ = 3;
		cost_ = 250;
		addAction(MOVEPRESET_ONE);
		addAction(MOVEPRESET_ZERO);
		break;
	case PROGRAM_BITMAN2:
		name_ = "Bit-Woman";
		description_ = "Makes distant sectors of the grid appear or disappear";
		maxMoves_ = 4;
		maxHealth_ = 2;
		cost_ = 1000;
		addAction(MOVEPRESET_CONSTRUCTOR);
		addAction(MOVEPRESET_DECONSTRUCTOR);
		break;
	case PROGRAM_BLACKWIDOW:
		name_ = "Black Widow";
		description_ = "Speedier and creepier";
		maxMoves_ = 4;
		maxHealth_ = 3;
		cost_ = 2000;
		addAction(MOVEPRESET_BYTE);
		addAction(MOVEPRESET_PARALYZE);
		break;
	case PROGRAM_BOSS:
		name_ = "Boss";
		description_ = "Prepare to be owned";
		maxMoves_ = 6;
		maxHealth_ = 25;
		cost_ = 50000;
		addAction(MOVEPRESET_SHUTDOWN);
		break;
	case PROGRAM_BUG:
		name_ = "Bug";
		description_ = "Fast, cheap, and out of control";
		maxMoves_ = 5;
		maxHealth_ = 1;
		cost_ = 750;
		addAction(MOVEPRESET_GLITCH);
		break;
	case PROGRAM_BUG2:
		name_ = "MandelBug";
		description_ = "It's not a bug, it's a feature";
		maxMoves_ = 5;
		maxHealth_ = 1;
		cost_ = 3000;
		addAction(MOVEPRESET_FRACTAL_GLITCH);
		break;
	case PROGRAM_BUG3:
		name_ = "HeisenBug";
		description_ = "They can't kill what they can't catch";
		maxMoves_ = 5;
		maxHealth_ = 1;
		cost_ = 4000;
		addAction(MOVEPRESET_QUANTUM_GLITCH);
		break;
	case PROGRAM_CATAPULT:
		name_ = "Catapult";
		description_ = "Extreme-range mobile attacker";
		maxMoves_ = 2;
		maxHealth_ = 3;
		cost_ = 4000;
		addAction(MOVEPRESET_FLING);
		break;
	case PROGRAM_CLOG:
		name_ = "Clog.01";
		description_ = "Slows down hostile programs";
		maxMoves_ = 2;
		maxHealth_ = 4;
		cost_ = 1000;
		addAction(MOVEPRESET_LAG);
		break;
	case PROGRAM_CLOG2:
		name_ = "Clog.02";
		description_ = "Twice as effective as version .01";
		maxMoves_ = 2;
		maxHealth_ = 4;
		cost_ = 2000;
		addAction(MOVEPRESET_CHUG);
		break;
	case PROGRAM_CLOG3:
		name_ = "Clog.03";
		description_ = "Brings hostile programs to a halt";
		maxMoves_ = 2;
		maxHealth_ = 4;
		cost_ = 3500;
		addAction(MOVEPRESET_CHUG);
		addAction(MOVEPRESET_HANG);
		break;
	case PROGRAM_DATABOMB:
		name_ = "LogicBomb";
		description_ = "Self-destructing attack program";
		maxMoves_ = 3;
		maxHealth_ = 6;
		cost_ = 3500;
		addAction(MOVEPRESET_SELFDESTRUCT);
		break;
	case PROGRAM_DATADOCTOR:
		name_ = "Data Doctor";
		description_ = "Helps your programs grow";
		maxMoves_ = 4;
		maxHealth_ = 5;
		cost_ = 500;
		addAction(MOVEPRESET_GROW);
		break;
	case PROGRAM_DATADOCTOR2:
		name_ = "Data Doctor Pro";
		description_ = "Twice the expansion power of Data Doctor";
		maxMoves_ = 5;
		maxHealth_ = 8;
		cost_ = 1500;
		addAction(MOVEPRESET_MEGAGROW);
		addAction(MOVEPRESET_SURGERY);
		break;
	case PROGRAM_DOG:
		name_ = "Guard Pup";
		description_ = "A speedy little corporate cur";
		maxMoves_ = 3;
		maxHealth_ = 2;
		cost_ = 300;
		addAction(MOVEPRESET_BYTE);
		break;
	case PROGRAM_DOG2:
		name_ = "Guard Dog";
		description_ = "Who let the dogs out?";
		maxMoves_ = 3;
		maxHealth_ = 3;
		cost_ = 300;
		addAction(MOVEPRESET_BYTE);
		break;
	case PROGRAM_DOG3:
		name_ = "Attack Dog";
		description_ = "Ravenous and bloodthirsty corporate canine";
		maxMoves_ = 4;
		maxHealth_ = 7;
		cost_ = 300;
		addAction(MOVEPRESET_MEGABYTE);
		break;
	case PROGRAM_FIDDLE:
		name_ = "Fiddle";
		description_ = "Twiddle and tweak the power of your programs";
		maxMoves_ = 3;
		maxHealth_ = 3;
		cost_ = 2400;
		addAction(MOVEPRESET_TWEAK);
		addAction(MOVEPRESET_TWIDDLE);
		break;
	case PROGRAM_FIREWALL:
		name_ = "Fire Wall";
		description_ = "Keeps unwanted programs out of corporate sectors";
		maxMoves_ = 2;
		maxHealth_ = 20;
		cost_ = 300;
		addAction(MOVEPRESET_BURN);
		break;
	case PROGRAM_GOLEM:
		name_ = "Golem.mud";
		description_ = "Slow and steady attack program";
		maxMoves_ = 1;
		maxHealth_ = 5;
		cost_ = 1200;
		addAction(MOVEPRESET_THUMP);
		break;
	case PROGRAM_GOLEM2:
		name_ = "Golem.clay";
		description_ = "Clay is stronger than mud";
		maxMoves_ = 2;
		maxHealth_ = 6;
		cost_ = 3000;
		addAction(MOVEPRESET_BASH);
		break;
	case PROGRAM_GOLEM3:
		name_ = "Golem.stone";
		description_ = "Nothing can stand in its way";
		maxMoves_ = 3;
		maxHealth_ = 7;
		cost_ = 5000;
		addAction(MOVEPRESET_CRASH);
		break;
	case PROGRAM_HACK:
		name_ = "Hack";
		description_ = "Basic attack program";
		maxMoves_ = 2;
		maxHealth_ = 4;
		cost_ = 500;
		addAction(MOVEPRESET_SLICE);
		break;
	case PROGRAM_HACK2:
		name_ = "Hack 2.0";
		description_ = "Improved Hack: larger size and better attacks";
		maxMoves_ = 3;
		maxHealth_ = 4;
		cost_ = 1500;
		addAction(MOVEPRESET_SLICE);
		addAction(MOVEPRESET_DICE);
		break;
	case PROGRAM_HACK3:
		name_ = "Hack 3.0";
		description_ = "The top of the Hack series";
		maxMoves_ = 4;
		maxHealth_ = 4;
		cost_ = 3500;
		addAction(MOVEPRESET_SLICE);
		addAction(MOVEPRESET_MUTILATE);
		break;
	case PROGRAM_KAMIKAZEE:
		name_ = "BuzzBomb";
		description_ = "Fast and annoying";
		maxMoves_ = 8;
		maxHealth_ = 2;
		cost_ = 3500;
		addAction(MOVEPRESET_STING);
		addAction(MOVEPRESET_KAMIKAZEE);
		break;
	case PROGRAM_MEDIC:
		name_ = "Medic";
		description_ = "Grows your programs from a distance";
		maxMoves_ = 3;
		maxHealth_ = 3;
		cost_ = 1000;
		addAction(MOVEPRESET_HYPO);
		break;
	case PROGRAM_MEMHOG:
		name_ = "Memory Hog";
		description_ = "Massive memory-filling bloatware";
		maxMoves_ = 5;
		maxHealth_ = 30;
		cost_ = 300;
		break;
	case PROGRAM_MOBILETOWER:
		// TODO: check this program's description and moves again
		name_ = "Mobile Tower";
		description_ = "Immobile long-range program";
		maxMoves_ = 0;
		maxHealth_ = 1;
		cost_ = 1000;
		addAction(MOVEPRESET_LAUNCH);
		break;
	case PROGRAM_SATELLITE:
		name_ = "Satellite";
		description_ = "Short-range hard-hitting program";
		maxMoves_ = 1;
		maxHealth_ = 1;
		cost_ = 3500;
		addAction(MOVEPRESET_SCRAMBLE);
		break;
	case PROGRAM_SATELLITE2:
		name_ = "Laser Satellite";
		description_ = "Long-range hard-hitting program";
		maxMoves_ = 2;
		maxHealth_ = 1;
		cost_ = 5000;
		addAction(MOVEPRESET_MEGASCRAMBLE);
		break;
	case PROGRAM_SEEKER:
		name_ = "Seeker";
		description_ = "Solid distance attack program";
		maxMoves_ = 3;
		maxHealth_ = 3;
		cost_ = 1000;
		addAction(MOVEPRESET_PEEK);
		break;
	case PROGRAM_SEEKER2:
		name_ = "Seeker 2.0";
		description_ = "Seeker 2.0";
		maxMoves_ = 3;
		maxHealth_ = 4;
		cost_ = 2500;
		addAction(MOVEPRESET_POKE);
		break;
	case PROGRAM_SEEKER3:
		name_ = "Seeker 3.0";
		description_ = "Seeker with extra deletion power";
		maxMoves_ = 4;
		maxHealth_ = 5;
		cost_ = 4500;
		addAction(MOVEPRESET_POKE);
		addAction(MOVEPRESET_SEEK_AND_DESTROY);
		break;
	case PROGRAM_SLINGSHOT:
		name_ = "Slingshot";
		description_ = "Basic ranged attack program";
		maxMoves_ = 2;
		maxHealth_ = 2;
		cost_ = 750;
		addAction(MOVEPRESET_STONE);
		break;
	case PROGRAM_SONAR:
		name_ = "Sensor";
		description_ = "Immobile program eradicator";
		maxMoves_ = 0;
		maxHealth_ = 1;
		cost_ = 1750;
		addAction(MOVEPRESET_BLIP);
		break;
	case PROGRAM_SONAR2:
		name_ = "Radar";
		description_ = "Long-range program eradicator";
		maxMoves_ = 0;
		maxHealth_ = 1;
		cost_ = 1750;
		addAction(MOVEPRESET_PING);
		break;
	case PROGRAM_SONAR3:
		name_ = "Radar";
		description_ = "Deadly program eradicator";
		maxMoves_ = 0;
		maxHealth_ = 1;
		cost_ = 1750;
		addAction(MOVEPRESET_PONG);
		break;
	case PROGRAM_SPECS:
		name_ = "Guru";
		description_ = "Multipurpose software for the l33tist of the l33t";
		maxMoves_ = 2;
		maxHealth_ = 3;
		cost_ = 4500;
		addAction(MOVEPRESET_FIRE);
		addAction(MOVEPRESET_ICE);
		break;
	case PROGRAM_SUMO:
		name_ = "Sumo";
		description_ = "A massive and slow-moving powerhouse";
		maxMoves_ = 2;
		maxHealth_ = 12;
		cost_ = 4500;
		addAction(MOVEPRESET_DATASLAM);
		break;
	case PROGRAM_TARANTULA:
		name_ = "Tarantula";
		description_ = "Fast, with a venomous bite";
		maxMoves_ = 5;
		maxHealth_ = 3;
		cost_ = 3500;
		addAction(MOVEPRESET_MEGABYTE);
		addAction(MOVEPRESET_PARALYZE);
		break;
	case PROGRAM_TOWER:
		name_ = "Tower";
		description_ = "Immobile long-range program";
		maxMoves_ = 0;
		maxHealth_ = 1;
		cost_ = 1000;
		addAction(MOVEPRESET_LAUNCH);
		break;
	case PROGRAM_TURBO:
		name_ = "Turbo";
		description_ = "Speeds up your programs";
		maxMoves_ = 3;
		maxHealth_ = 3;
		cost_ = 1000;
		addAction(MOVEPRESET_BOOST);
		break;
	case PROGRAM_TURBO2:
		name_ = "Turbo Deluxe";
		description_ = "Slow and steady is for losers";
		maxMoves_ = 4;
		maxHealth_ = 4;
		cost_ = 1750;
		addAction(MOVEPRESET_MEGABOOST);
		break;
	case PROGRAM_TURBO3:
		name_ = "Turbo DLC";
		description_ = "Gotta go fast";
		maxMoves_ = 5;
		maxHealth_ = 5;
		cost_ = 3000;
		addAction(MOVEPRESET_GIGABOOST);
		break;
	case PROGRAM_WALKER:
		name_ = "Sentinel";
		description_ = "Corporate data defender";
		maxMoves_ = 1;
		maxHealth_ = 3;
		cost_ = 300;
		addAction(MOVEPRESET_CUT);
		break;
	case PROGRAM_WALKER2:
		name_ = "Sentinel 2.0";
		description_ = "Improved corporate data defender";
		maxMoves_ = 2;
		maxHealth_ = 4;
		cost_ = 300;
		addAction(MOVEPRESET_CUT);
		break;
	case PROGRAM_WALKER3:
		name_ = "Sentinel 3.0";
		description_ = "Top of the line in corporate data defense";
		maxMoves_ = 2;
		maxHealth_ = 4;
		cost_ = 300;
		addAction(MOVEPRESET_TASER);
		break;
	case PROGRAM_WARDEN:
		name_ = "Warden";
		description_ = "Slow and steady corporate attack program";
		maxMoves_ = 1;
		maxHealth_ = 5;
		cost_ = 2500;
		addAction(MOVEPRESET_THUMP);
		break;
	case PROGRAM_WARDEN2:
		name_ = "Warden+";
		description_ = "Get out of its way";
		maxMoves_ = 2;
		maxHealth_ = 6;
		cost_ = 3500;
		addAction(MOVEPRESET_BASH);
		break;
	case PROGRAM_WARDEN3:
		name_ = "Warden++";
		description_ = "The last word in corporate security";
		maxMoves_ = 3;
		maxHealth_ = 7;
		cost_ = 5000;
		addAction(MOVEPRESET_CRASH);
		break;
	case PROGRAM_WATCHMAN:
		name_ = "Watchman";
		description_ = "Basic ranged attack program";
		maxMoves_ = 1;
		maxHealth_ = 2;
		cost_ = 300;
		addAction(MOVEPRESET_PHASER);
		break;
	case PROGRAM_WATCHMAN2:
		name_ = "Watchman X";
		description_ = "Improved version of Watchman";
		maxMoves_ = 1;
		maxHealth_ = 4;
		cost_ = 300;
		addAction(MOVEPRESET_PHASER);
		break;
	case PROGRAM_WATCHMAN3:
		name_ = "Watchman SP";
		description_ = "QUI CUSTODIET IPSOS CUSTODES?";
		maxMoves_ = 1;
		maxHealth_ = 4;
		cost_ = 300;
		addAction(MOVEPRESET_PHOTON);
		break;
	case PROGRAM_WIZARD:
		name_ = "Wizard";
		description_ = "Pay no attention to the man behind the curtain";
		maxMoves_ = 3;
		maxHealth_ = 4;
		cost_ = 300;
		addAction(MOVEPRESET_SCORCH);
		addAction(MOVEPRESET_STRETCH);
		break;
	case PROGRAM_WOLFSPIDER:
		name_ = "Wolf Spider";
		description_ = "Speedy and creepy little program";
		maxMoves_ = 3;
		maxHealth_ = 3;
		cost_ = 750;
		addAction(MOVEPRESET_BYTE);
		break;
	case PROGRAM_MENTALIST:
		name_ = "Mentalist";
		description_ = "A master manipulator of the mind";
		maxMoves_ = 1;
		maxHealth_ = 2;
		cost_ = 10000;
		addAction(MOVEPRESET_FEAR);
		addAction(MOVEPRESET_COURAGE);
		break;
	}

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

void ProgramMirror::addAction(MOVEPRESET p) {
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