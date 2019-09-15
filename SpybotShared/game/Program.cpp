#include "Standard.h"
#include "Program.h"

#include "Global.h"
#include "Player.h"
#include "ProgramAction.h"

Program::Program(PROGRAM type, int team, Coord head) {
	this->team = team;
	this->type = type;
	color[0] = rand() % 255;
	color[1] = rand() % 255;
	color[2] = rand() % 255;
	actionList = new LinkedList<ProgramAction*>();
	tiles = new LinkedList<Coord*>();
	tiles->addFirst(new Coord(head));

	if (this->type == PROGRAM_CUSTOM) {
		return;
	}

	switch (this->type) {
	case PROGRAM_NONE:
	case PROGRAM_NUM_PROGTYPES:
		printf("ERROR: trying to instantiate an invalid program type");
		exit(1);
		break;
	case PROGRAM_CUSTOM:
		break;
	case PROGRAM_BALLISTA:
		name = "Ballista";
		description = "Extreme-range attack program";
		maxMoves = 1;
		maxHealth = 2;
		cost = 3000;
		addAction(MOVEPRESET_FLING);
		break;
	case PROGRAM_BITMAN:
		name = "Bit-Man";
		description = "Makes sectors of the grid appear or disappear";
		maxMoves = 3;
		maxHealth = 3;
		cost = 250;
		addAction(MOVEPRESET_ONE);
		addAction(MOVEPRESET_ZERO);
		break;
	case PROGRAM_BITMAN2:
		name = "Bit-Woman";
		description = "Makes distant sectors of the grid appear or disappear";
		maxMoves = 4;
		maxHealth = 2;
		cost = 1000;
		addAction(MOVEPRESET_CONSTRUCTOR);
		addAction(MOVEPRESET_DECONSTRUCTOR);
		break;
	case PROGRAM_BLACKWIDOW:
		name = "Black Widow";
		description = "Speedier and creepier";
		maxMoves = 4;
		maxHealth = 3;
		cost = 2000;
		addAction(MOVEPRESET_BYTE);
		addAction(MOVEPRESET_PARALYZE);
		break;
	case PROGRAM_BOSS:
		name = "Boss";
		description = "Prepare to be owned";
		maxMoves = 6;
		maxHealth = 25;
		cost = 50000;
		addAction(MOVEPRESET_SHUTDOWN);
		break;
	case PROGRAM_BUG:
		name = "Bug";
		description = "Fast, cheap, and out of control";
		maxMoves = 5;
		maxHealth = 1;
		cost = 750;
		addAction(MOVEPRESET_GLITCH);
		break;
	case PROGRAM_BUG2:
		name = "MandelBug";
		description = "It's not a bug, it's a feature";
		maxMoves = 5;
		maxHealth = 1;
		cost = 3000;
		addAction(MOVEPRESET_FRACTAL_GLITCH);
		break;
	case PROGRAM_BUG3:
		name = "HeisenBug";
		description = "They can't kill what they can't catch";
		maxMoves = 5;
		maxHealth = 1;
		cost = 4000;
		addAction(MOVEPRESET_QUANTUM_GLITCH);
		break;
	case PROGRAM_CATAPULT:
		name = "Catapult";
		description = "Extreme-range mobile attacker";
		maxMoves = 2;
		maxHealth = 3;
		cost = 4000;
		addAction(MOVEPRESET_FLING);
		break;
	case PROGRAM_CLOG:
		name = "Clog.01";
		description = "Slows down hostile programs";
		maxMoves = 2;
		maxHealth = 4;
		cost = 1000;
		addAction(MOVEPRESET_LAG);
		break;
	case PROGRAM_CLOG2:
		name = "Clog.02";
		description = "Twice as effective as version .01";
		maxMoves = 2;
		maxHealth = 4;
		cost = 2000;
		addAction(MOVEPRESET_CHUG);
		break;
	case PROGRAM_CLOG3:
		name = "Clog.03";
		description = "Brings hostile programs to a halt";
		maxMoves = 2;
		maxHealth = 4;
		cost = 3500;
		addAction(MOVEPRESET_CHUG);
		addAction(MOVEPRESET_HANG);
		break;
	case PROGRAM_DATABOMB:
		name = "LogicBomb";
		description = "Self-destructing attack program";
		maxMoves = 3;
		maxHealth = 6;
		cost = 3500;
		addAction(MOVEPRESET_SELFDESTRUCT);
		break;
	case PROGRAM_DATADOCTOR:
		name = "Data Doctor";
		description = "Helps your programs grow";
		maxMoves = 4;
		maxHealth = 5;
		cost = 500;
		addAction(MOVEPRESET_GROW);
		break;
	case PROGRAM_DATADOCTOR2:
		name = "Data Doctor Pro";
		description = "Twice the expansion power of Data Doctor";
		maxMoves = 5;
		maxHealth = 8;
		cost = 1500;
		addAction(MOVEPRESET_MEGAGROW);
		addAction(MOVEPRESET_SURGERY);
		break;
	case PROGRAM_DOG:
		name = "Guard Pup";
		description = "A speedy little corporate cur";
		maxMoves = 3;
		maxHealth = 2;
		cost = 300;
		addAction(MOVEPRESET_BYTE);
		break;
	case PROGRAM_DOG2:
		name = "Guard Dog";
		description = "Who let the dogs out?";
		maxMoves = 3;
		maxHealth = 3;
		cost = 300;
		addAction(MOVEPRESET_BYTE);
		break;
	case PROGRAM_DOG3:
		name = "Attack Dog";
		description = "Ravenous and bloodthirsty corporate canine";
		maxMoves = 4;
		maxHealth = 7;
		cost = 300;
		addAction(MOVEPRESET_MEGABYTE);
		break;
	case PROGRAM_FIDDLE:
		name = "Fiddle";
		description = "Twiddle and tweak the power of your programs";
		maxMoves = 3;
		maxHealth = 3;
		cost = 2400;
		addAction(MOVEPRESET_TWEAK);
		addAction(MOVEPRESET_TWIDDLE);
		break;
	case PROGRAM_FIREWALL:
		name = "Fire Wall";
		description = "Keeps unwanted programs out of corporate sectors";
		maxMoves = 2;
		maxHealth = 20;
		cost = 300;
		addAction(MOVEPRESET_BURN);
		break;
	case PROGRAM_GOLEM:
		name = "Golem.mud";
		description = "Slow and steady attack program";
		maxMoves = 1;
		maxHealth = 5;
		cost = 1200;
		addAction(MOVEPRESET_THUMP);
		break;
	case PROGRAM_GOLEM2:
		name = "Golem.clay";
		description = "Clay is stronger than mud";
		maxMoves = 2;
		maxHealth = 6;
		cost = 3000;
		addAction(MOVEPRESET_BASH);
		break;
	case PROGRAM_GOLEM3:
		name = "Golem.stone";
		description = "Nothing can stand in its way";
		maxMoves = 3;
		maxHealth = 7;
		cost = 5000;
		addAction(MOVEPRESET_CRASH);
		break;
	case PROGRAM_HACK:
		name = "Hack";
		description = "Basic attack program";
		maxMoves = 2;
		maxHealth = 4;
		cost = 500;
		addAction(MOVEPRESET_SLICE);
		break;
	case PROGRAM_HACK2:
		name = "Hack 2.0";
		description = "Improved Hack: larger size and better attacks";
		maxMoves = 3;
		maxHealth = 4;
		cost = 1500;
		addAction(MOVEPRESET_SLICE);
		addAction(MOVEPRESET_DICE);
		break;
	case PROGRAM_HACK3:
		name = "Hack 3.0";
		description = "The top of the Hack series";
		maxMoves = 4;
		maxHealth = 4;
		cost = 3500;
		addAction(MOVEPRESET_SLICE);
		addAction(MOVEPRESET_MUTILATE);
		break;
	case PROGRAM_KAMIKAZEE:
		name = "BuzzBomb";
		description = "Fast and annoying";
		maxMoves = 8;
		maxHealth = 2;
		cost = 3500;
		addAction(MOVEPRESET_STING);
		addAction(MOVEPRESET_KAMIKAZEE);
		break;
	case PROGRAM_MEDIC:
		name = "Medic";
		description = "Grows your programs from a distance";
		maxMoves = 3;
		maxHealth = 3;
		cost = 1000;
		addAction(MOVEPRESET_HYPO);
		break;
	case PROGRAM_MEMHOG:
		name = "Memory Hog";
		description = "Massive memory-filling bloatware";
		maxMoves = 5;
		maxHealth = 30;
		cost = 300;
		break;
	case PROGRAM_MOBILETOWER:
		// TODO: check this program's description and moves again
		name = "Mobile Tower";
		description = "Immobile long-range program";
		maxMoves = 0;
		maxHealth = 1;
		cost = 1000;
		addAction(MOVEPRESET_LAUNCH);
		break;
	case PROGRAM_SATELLITE:
		name = "Satellite";
		description = "Short-range hard-hitting program";
		maxMoves = 1;
		maxHealth = 1;
		cost = 3500;
		addAction(MOVEPRESET_SCRAMBLE);
		break;
	case PROGRAM_SATELLITE2:
		name = "Laser Satellite";
		description = "Long-range hard-hitting program";
		maxMoves = 2;
		maxHealth = 1;
		cost = 5000;
		addAction(MOVEPRESET_MEGASCRAMBLE);
		break;
	case PROGRAM_SEEKER:
		name = "Seeker";
		description = "Solid distance attack program";
		maxMoves = 3;
		maxHealth = 3;
		cost = 1000;
		addAction(MOVEPRESET_PEEK);
		break;
	case PROGRAM_SEEKER2:
		name = "Seeker 2.0";
		description = "Seeker 2.0";
		maxMoves = 3;
		maxHealth = 4;
		cost = 2500;
		addAction(MOVEPRESET_POKE);
		break;
	case PROGRAM_SEEKER3:
		name = "Seeker 3.0";
		description = "Seeker with extra deletion power";
		maxMoves = 4;
		maxHealth = 5;
		cost = 4500;
		addAction(MOVEPRESET_POKE);
		addAction(MOVEPRESET_SEEK_AND_DESTROY);
		break;
	case PROGRAM_SLINGSHOT:
		name = "Slingshot";
		description = "Basic ranged attack program";
		maxMoves = 2;
		maxHealth = 2;
		cost = 750;
		addAction(MOVEPRESET_STONE);
		break;
	case PROGRAM_SONAR:
		name = "Sensor";
		description = "Immobile program eradicator";
		maxMoves = 0;
		maxHealth = 1;
		cost = 1750;
		addAction(MOVEPRESET_BLIP);
		break;
	case PROGRAM_SONAR2:
		name = "Radar";
		description = "Long-range program eradicator";
		maxMoves = 0;
		maxHealth = 1;
		cost = 1750;
		addAction(MOVEPRESET_PING);
		break;
	case PROGRAM_SONAR3:
		name = "Radar";
		description = "Deadly program eradicator";
		maxMoves = 0;
		maxHealth = 1;
		cost = 1750;
		addAction(MOVEPRESET_PONG);
		break;
	case PROGRAM_SPECS:
		name = "Guru";
		description = "Multipurpose software for the l33tist of the l33t";
		maxMoves = 2;
		maxHealth = 3;
		cost = 4500;
		addAction(MOVEPRESET_FIRE);
		addAction(MOVEPRESET_ICE);
		break;
	case PROGRAM_SUMO:
		name = "Sumo";
		description = "A massive and slow-moving powerhouse";
		maxMoves = 2;
		maxHealth = 12;
		cost = 4500;
		addAction(MOVEPRESET_DATASLAM);
		break;
	case PROGRAM_TARANTULA:
		name = "Tarantula";
		description = "Fast, with a venomous bite";
		maxMoves = 5;
		maxHealth = 3;
		cost = 3500;
		addAction(MOVEPRESET_MEGABYTE);
		addAction(MOVEPRESET_PARALYZE);
		break;
	case PROGRAM_TOWER:
		name = "Tower";
		description = "Immobile long-range program";
		maxMoves = 0;
		maxHealth = 1;
		cost = 1000;
		addAction(MOVEPRESET_LAUNCH);
		break;
	case PROGRAM_TURBO:
		name = "Turbo";
		description = "Speeds up your programs";
		maxMoves = 3;
		maxHealth = 3;
		cost = 1000;
		addAction(MOVEPRESET_BOOST);
		break;
	case PROGRAM_TURBO2:
		name = "Turbo Deluxe";
		description = "Slow and steady is for losers";
		maxMoves = 4;
		maxHealth = 4;
		cost = 1750;
		addAction(MOVEPRESET_MEGABOOST);
		break;
	case PROGRAM_TURBO3:
		name = "Turbo DLC";
		description = "Gotta go fast";
		maxMoves = 5;
		maxHealth = 5;
		cost = 3000;
		addAction(MOVEPRESET_GIGABOOST);
		break;
	case PROGRAM_WALKER:
		name = "Sentinel";
		description = "Corporate data defender";
		maxMoves = 1;
		maxHealth = 3;
		cost = 300;
		addAction(MOVEPRESET_CUT);
		break;
	case PROGRAM_WALKER2:
		name = "Sentinel 2.0";
		description = "Improved corporate data defender";
		maxMoves = 2;
		maxHealth = 4;
		cost = 300;
		addAction(MOVEPRESET_CUT);
		break;
	case PROGRAM_WALKER3:
		name = "Sentinel 3.0";
		description = "Top of the line in corporate data defense";
		maxMoves = 2;
		maxHealth = 4;
		cost = 300;
		addAction(MOVEPRESET_TASER);
		break;
	case PROGRAM_WARDEN:
		name = "Warden";
		description = "Slow and steady corporate attack program";
		maxMoves = 1;
		maxHealth = 5;
		cost = 2500;
		addAction(MOVEPRESET_THUMP);
		break;
	case PROGRAM_WARDEN2:
		name = "Warden+";
		description = "Get out of its way";
		maxMoves = 2;
		maxHealth = 6;
		cost = 3500;
		addAction(MOVEPRESET_BASH);
		break;
	case PROGRAM_WARDEN3:
		name = "Warden++";
		description = "The last word in corporate security";
		maxMoves = 3;
		maxHealth = 7;
		cost = 5000;
		addAction(MOVEPRESET_CRASH);
		break;
	case PROGRAM_WATCHMAN:
		name = "Watchman";
		description = "Basic ranged attack program";
		maxMoves = 1;
		maxHealth = 2;
		cost = 300;
		addAction(MOVEPRESET_PHASER);
		break;
	case PROGRAM_WATCHMAN2:
		name = "Watchman X";
		description = "Improved version of Watchman";
		maxMoves = 1;
		maxHealth = 4;
		cost = 300;
		addAction(MOVEPRESET_PHASER);
		break;
	case PROGRAM_WATCHMAN3:
		name = "Watchman SP";
		description = "QUI CUSTODIET IPSOS CUSTODES?";
		maxMoves = 1;
		maxHealth = 4;
		cost = 300;
		addAction(MOVEPRESET_PHOTON);
		break;
	case PROGRAM_WIZARD:
		name = "Wizard";
		description = "Pay no attention to the man behind the curtain";
		maxMoves = 3;
		maxHealth = 4;
		cost = 300;
		addAction(MOVEPRESET_SCORCH);
		addAction(MOVEPRESET_STRETCH);
		break;
	case PROGRAM_WOLFSPIDER:
		name = "Wolf Spider";
		description = "Speedy and creepy little program";
		maxMoves = 3;
		maxHealth = 3;
		cost = 750;
		addAction(MOVEPRESET_BYTE);
		break;
	}

	moves = maxMoves;
	actionsLeft = 1;
}

Program::~Program() {
	while (tiles->getLength() > 0) {
		Coord* c = tiles->poll();
		delete c;
	}
	delete tiles;

	while (actionList->getLength() > 0) {
		ProgramAction* m = actionList->poll();
		delete m;
	}
	delete actionList;

	if (_debug >= DEBUG_NORMAL) {
		printf("Program '%s' deleted\n", name.c_str());
	}
}

Coord Program::getCore() {
	return *tiles->getFirst();
}

int Program::getColor(int n) {
	if (n < 0 || n > 2) {
		return 0;
	}

	return color[n];
}

void Program::setColor(int r, int g, int b) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

int Program::getTeam() {
	return owner->getTeam();
}

int Program::getHealth() {
	return tiles->getLength();
}

int Program::getMaxHealth() {
	return maxHealth;
}

PROGRAM Program::getType() {
	return type;
}

int Program::getMoves() {
	return moves;
}

int Program::getMaxMoves() {
	return maxMoves;
}

void Program::setType(PROGRAM i) {
	type = i;
}

void Program::setMaxHealth(int i) {
	maxHealth = i;
}

void Program::setMoves(int i) {
	if (i < 0) moves = 0;
	else moves = i;
}

void Program::setMaxMoves(int i) {
	maxMoves = i;
}

std::string Program::getName() {
	return name;
}

void Program::setName(std::string n) {
	name = n;
}

void Program::addAction(MOVEPRESET p) {
	actionList->addLast(new ProgramAction(p));
}

void Program::endTurn() {
	actionsLeft = 1;
	moves = maxMoves;
}

LinkedList<ProgramAction*>* Program::getActions() {
	return actionList;
}

Coord Program::getHead() {
	return *tiles->getFirst();
}

Coord Program::getTail() {
	return *tiles->getLast();
}

void Program::moveTo(Coord pos) {
	// decrement number of moves left
	moves--;

	// check if the tile to move to is already occupied by this program
	for (int i = 0; i < tiles->getLength(); i++) {
		Coord* curr = tiles->getObjectAt(i);
		if (curr->x == pos.x && curr->y == pos.y) {
			tiles->removeObjectAt(i);
			tiles->addFirst(curr);
			return;
		}
	}

	// if this program is at max health
	if (tiles->getLength() == maxHealth) delete tiles->removeLast();

	// add a new head
	tiles->addFirst(new Coord(pos));
}

void Program::setCore(Coord pos) {
	moveTo(pos);
}

void Program::addTail(Coord pos) {
	// check if the tail to add is already occupied by this program
	for (int i = 0; i < tiles->getLength(); i++) {
		Coord curr = *tiles->getObjectAt(i);
		if (curr.x == pos.x && curr.y == pos.y) return;
	}

	// if the given coords aren't occupied by this program, add it
	if (tiles->getLength() < maxHealth) tiles->addLast(new Coord(pos));
}

int Program::getActionsLeft() {
	return actionsLeft;
}

void Program::setActionsLeft(int i) {
	actionsLeft = i;
}

bool Program::isDone() {
	if (moves == 0 && actionsLeft == 0) return true;
	else return false;
}

Coord* Program::popTail() {
	return tiles->removeLast();
}

LinkedList<Coord*>* Program::getTiles() {
	return tiles;
}

Player* Program::getOwner() {
	return owner;
}

void Program::setOwner(Player* p) {
	owner = p;
}

int Program::getProgramID() {
	return programID_;
}

void Program::setProgramID(int progID) {
	programID_ = progID;
}

ProgramAction* Program::getActionByID(int actionID) {
	Iterator<ProgramAction*> it = actionList->getIterator();
	while (it.hasNext()) {
		ProgramAction* curr = it.next();
		if (curr->actionID == actionID)
			return curr;
	}

	return NULL;
}