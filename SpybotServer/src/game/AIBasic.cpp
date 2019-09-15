#include "Standard.h"
#include "AIBasic.h"

#include "Program.h"
#include "Main.h"
#include "Game.h"
#include "ProgramAction.h"
#include "Player.h"

AIBasic::AIBasic(Player* owner)
    : AICore(owner)
{
    //ctor
}

AIBasic::~AIBasic()
{
    //dtor
}

bool AIBasic::actStep()
{
    // select the next usable program
    if (_debug >= DEBUG_NORMAL) 
		printf("stepping aiBasic %x: ", (unsigned int)this);
    if (owner_->getSelectedProgram() == NULL || owner_->getSelectedProgram()->isDone())
    {
        if (_debug >= DEBUG_NORMAL) 
			printf("selecting next usable program... ");
        Program* myProg = getNextUsableProgram();
        if (myProg == NULL)
        {
            if (_debug >= DEBUG_NORMAL) 
				printf("found no remaining usable programs\n");
            return false;
        }
        else
        {
            if (_debug >= DEBUG_NORMAL) 
				printf("found %s\n", myProg->getName().c_str());
            owner_->setSelectedProgram(myProg);
            return true;
        }
    }

    // if it still has moves left, move it
    if (owner_->getSelectedProgram()->getMoves() > 0)
    {
        if (_debug >= DEBUG_NORMAL) 
			printf("moving %s to... ", owner_->getSelectedProgram()->getName().c_str());

        // get the nearest enemy program to this program
        Program* enemyProg = getNearestEnemyProgramAbsoluteDist(NULL);

        // check if all enemies are dead
        if (enemyProg == NULL)
        {
            if (_debug >= DEBUG_NORMAL) 
				printf("nowhere; no enemies left\n");
			owner_->getSelectedProgram()->setMoves(owner_->getSelectedProgram()->getMoves() - 1);
            return false;
        }

        // get a naive best move towards the nearest enemy program
        Coord* bestMove = naiveStepTowardsProgram(enemyProg);

        // use the naive best move
        if (bestMove == NULL)
        {
            if (_debug >= DEBUG_NORMAL) 
				printf("nowhere; move blocked\n");
			owner_->getSelectedProgram()->setMoves(owner_->getSelectedProgram()->getMoves() - 1);
        }
        else
        {
            if (_debug >= DEBUG_NORMAL)
				printf("%i,%i\n", bestMove->x, bestMove->y);
            owner_->moveSelectedProgramBy(*bestMove);
            delete bestMove;
        }
        return true;
    }

    // if it still has actions left, use one
    if (owner_->getSelectedProgram()->getActionsLeft() > 0)
    {
        if (owner_->getSelectedAction() == NULL)
        {
            if (_debug >= DEBUG_NORMAL) 
				printf("picking one of %s's actions... ", owner_->getSelectedProgram()->getName().c_str());
            ProgramAction* progAct = owner_->getSelectedProgram()->getActions()->getFirst();
            if (progAct == NULL)
            {
                if (_debug >= DEBUG_NORMAL) 
					printf("found no available actions\n");
				owner_->getSelectedProgram()->setActionsLeft(0);
            }
            else
            {
                if (_debug >= DEBUG_NORMAL) 
					printf("picked %s\n", progAct->name.c_str());
                owner_->setSelectedAction(progAct);
            }
        }
        else
        {
            if (_debug >= DEBUG_NORMAL) 
				printf("using %s's action %s...", owner_->getSelectedProgram()->getName().c_str(), owner_->getSelectedProgram()->getName().c_str());
            Coord c = {-1, -1};
            for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
                if (owner_->getSelectedActionDist({x,y}) != -1 &&
                    owner_->getGame()->getProgramAt({x,y}) != NULL &&
                    owner_->getGame()->getProgramAt({x,y})->getTeam() != owner_->getTeam())
                {
                    c = {x, y};
                }
            if (c == Coord{-1, -1})
            {
                if (_debug >= DEBUG_NORMAL) 
					printf("nowhere- no enemy progs in range\n");
            }
            else
            {
                if (_debug >= DEBUG_NORMAL) 
					printf("at %i,%i\n", c.x, c.y);
                owner_->useSelectedActionAt(c);
            }

			owner_->getSelectedProgram()->setActionsLeft(0);
        }
        return true;
    }

    // if this method has gotten this far, there's been an error
    printf("err: tried to do something with a completely exhausted program\n");
    return false;
}
