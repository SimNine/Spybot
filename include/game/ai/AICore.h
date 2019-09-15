#ifndef AICORE_H
#define AICORE_H

#include "Game.h"
#include "Player.h"

class AICore : public Player
{
    public:
        AICore(Game*, int);
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
    private:
};

#endif // AICORE_H
