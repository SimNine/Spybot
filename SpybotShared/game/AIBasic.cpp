#include "Standard.h"
#include "AIBasic.h"

#include "Global.h"
#include "Program.h"
#include "Game.h"
#include "ProgramAction.h"
#include "Player.h"

AIBasic::AIBasic(Player* owner)
	: AICore(owner) {
	//ctor
}

AIBasic::~AIBasic() {
	//dtor
}

bool AIBasic::actStep() {
	// select the next usable program
	if (_debug >= DEBUG_NORMAL)
		log("stepping aiBasic " + to_string((unsigned int)this) + ": ");
	if (owner_->getSelectedProgram() == NULL || owner_->getSelectedProgram()->isDone()) {
		if (_debug >= DEBUG_NORMAL)
			log("selecting next usable program... ");
		Program* myProg = getNextUsableProgram();
		if (myProg == NULL) {
			if (_debug >= DEBUG_NORMAL)
				log("found no remaining usable programs\n");
			return false;
		} else {
			if (_debug >= DEBUG_NORMAL)
				log("found " + myProg->getName() + "\n");
			owner_->setSelectedProgram(myProg);
			return true;
		}
	}

	// if it still has moves left, move it
	if (owner_->getSelectedProgram()->getMoves() > 0) {
		if (_debug >= DEBUG_NORMAL)
			log("moving " + owner_->getSelectedProgram()->getName() + " to... ");

		// get the nearest enemy program to this program
		Program* enemyProg = getNearestEnemyProgramAbsoluteDist(NULL);

		// check if all enemies are dead
		if (enemyProg == NULL) {
			if (_debug >= DEBUG_NORMAL)
				log("nowhere; no enemies left\n");
			owner_->getSelectedProgram()->setMoves(owner_->getSelectedProgram()->getMoves() - 1);
			return false;
		}

		// get a naive best move towards the nearest enemy program
		Coord* bestMove = naiveStepTowardsProgram(enemyProg);

		// use the naive best move
		if (bestMove == NULL) {
			if (_debug >= DEBUG_NORMAL)
				log("nowhere; move blocked\n");
			owner_->getSelectedProgram()->setMoves(owner_->getSelectedProgram()->getMoves() - 1);
		} else {
			if (_debug >= DEBUG_NORMAL)
				log(to_string(bestMove->x) + "," + to_string(bestMove->y) + "\n");
			owner_->moveSelectedProgramBy(*bestMove);
			delete bestMove;
		}
		return true;
	}

	// if it still has actions left, use one
	if (owner_->getSelectedProgram()->getActionsLeft() > 0) {
		if (owner_->getSelectedAction() == NULL) {
			if (_debug >= DEBUG_NORMAL)
				log("picking one of " + owner_->getSelectedProgram()->getName() + "'s actions... ");
			ProgramAction* progAct = owner_->getSelectedProgram()->getActions()->getFirst();
			if (progAct == NULL) {
				if (_debug >= DEBUG_NORMAL)
					log("found no available actions\n");
				owner_->getSelectedProgram()->setActionsLeft(0);
			} else {
				if (_debug >= DEBUG_NORMAL)
					log("picked " + progAct->name_ + "\n");
				owner_->setSelectedAction(progAct);
			}
		} else {
			if (_debug >= DEBUG_NORMAL)
				log("using " + owner_->getSelectedProgram()->getName() + "'s action " + owner_->getSelectedAction()->name_ + "...");
			Coord c = NULLCOORD;
			for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
				if (owner_->getSelectedActionDist({ x,y }) != -1 &&
					owner_->getGame()->getProgramAt({ x,y }) != NULL &&
					owner_->getGame()->getProgramAt({ x,y })->getTeam() != owner_->getTeam()) {
					c = { x, y };
				}
			if (c == NULLCOORD) {
				if (_debug >= DEBUG_NORMAL)
					log("nowhere- no enemy progs in range\n");
			} else {
				if (_debug >= DEBUG_NORMAL)
					log("at " + to_string(c.x) + "," + to_string(c.y) + "\n");
				owner_->useSelectedActionAt(c);
			}

			owner_->getSelectedProgram()->setActionsLeft(0);
		}
		return true;
	}

	// if this method has gotten this far, there's been an error
	log("err: tried to do something with a completely exhausted program\n");
	return false;
}
