#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Program;
class Player;

class AICore {
public:
	AICore(Player* owner);
	virtual ~AICore();

	// do a certain number of actions
	void act(int);
protected:
	// do one action step
	virtual bool actStep() = 0;

	// helper methods for AI subclasses
	Program* getNextUsableProgram();
	Program* getNearestEnemyProgramAbsoluteDist(Player*);
	Program* getNearestEnemyProgramAStarDist(Player*);
	Coord* idealStepTowardsProgram(Program*);
	Coord* naiveStepTowardsProgram(Program*);
	LinkedList<Program*>* getAllEnemyPrograms();

	// owner of this mind
	Player* owner_;
private:
};