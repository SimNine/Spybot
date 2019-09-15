#include <stdlib.h>

#include "AIBasic.h"
#include "MiscUtil.h"
#include "Global.h"

AIBasic::AIBasic(Game* g, int t)
    : AICore(g, t)
{
    //ctor
}

AIBasic::~AIBasic()
{
    //dtor
}

bool AIBasic::actStep()
{
    // get the next usable program
    if (debug >= DEBUG_NORMAL) printf("stepping aiBasic %x: ", (unsigned int)this);
    if (selectedProgram == NULL || selectedProgram->isDone())
    {
        if (debug >= DEBUG_NORMAL) printf("selecting next usable program... ");
        Program* myProg = getNextUsableProgram();
        if (myProg == NULL)
        {
            if (debug >= DEBUG_NORMAL) printf("found no remaining usable programs\n");
            return false;
        }
        else
        {
            if (debug >= DEBUG_NORMAL) printf("found %s\n", myProg->getName().c_str());
            setSelectedProgram(myProg);
            return true;
        }
    }

    // if it still has moves left, move it
    if (selectedProgram->getMoves() > 0)
    {
        if (debug >= DEBUG_NORMAL) printf("moving %s to... ", selectedProgram->getName().c_str());

        // get the nearest enemy program to this program
        Program* enemyProg = getNearestEnemyProgramAbsoluteDist(NULL);

        // check if all enemies are dead
        if (enemyProg == NULL)
        {
            if (debug >= DEBUG_NORMAL) printf("nowhere; no enemies left\n");
            selectedProgram->setMoves(selectedProgram->getMoves() - 1);
            return false;
        }

        // get a naive best move towards the nearest enemy program
        Coord* bestMove = naiveStepTowardsProgram(enemyProg);

        // use the naive best move
        if (bestMove == NULL)
        {
            if (debug >= DEBUG_NORMAL) printf("nowhere; move blocked\n");
            selectedProgram->setMoves(selectedProgram->getMoves() - 1);
        }
        else
        {
            if (debug >= DEBUG_NORMAL) printf("%i,%i\n", bestMove->x, bestMove->y);
            moveSelectedProgramBy(*bestMove);
            delete bestMove;
        }
        return true;
    }

    // if it still has actions left, use one
    if (selectedProgram->getActionsLeft() > 0)
    {
        if (selectedAction == NULL)
        {
            if (debug >= DEBUG_NORMAL) printf("picking one of %s's actions... ", selectedProgram->getName().c_str());
            ProgramAction* progAct = selectedProgram->getActions()->getFirst();
            if (progAct == NULL)
            {
                if (debug >= DEBUG_NORMAL) printf("found no available actions\n");
                selectedProgram->setActionsLeft(0);
            }
            else
            {
                if (debug >= DEBUG_NORMAL) printf("picked %s\n", progAct->name.c_str());
                setSelectedAction(progAct);
            }
        }
        else
        {
            if (debug >= DEBUG_NORMAL) printf("using %s's action %s...", selectedProgram->getName().c_str(), selectedAction->name.c_str());
            Coord c = {-1, -1};
            for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
                if (getSelectedActionDist({x,y}) != -1 &&
                    game->getProgramAt({x,y}) != NULL &&
                    game->getProgramAt({x,y})->getTeam() != team)
                {
                    c = {x, y};
                }
            if (c == Coord{-1, -1})
            {
                if (debug >= DEBUG_NORMAL) printf("nowhere- no enemy progs in range\n");
            }
            else
            {
                if (debug >= DEBUG_NORMAL) printf("at %i,%i\n", c.x, c.y);
                useSelectedActionAt(c);
            }

            selectedProgram->setActionsLeft(0);
        }
        return true;
    }

    // if this method has gotten this far, there's been an error
    printf("err: tried to do something with a completely exhausted program\n");
    return false;
}
