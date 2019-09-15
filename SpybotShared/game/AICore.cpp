#include "Standard.h"
#include "AICore.h"

#include "Game.h"
#include "Program.h"
#include "MiscUtil.h"
#include "Player.h"
#include "Team.h"

AICore::AICore(Player* owner) {
	owner_ = owner;
}

AICore::~AICore() {
	//dtor
}

void AICore::act(int num) {
	//log("agent on team %i carrying out %i action steps...\n", team, num);
	if (num == -1) {
		bool b = true;
		while (b)
			b = actStep();
		owner_->setDoneTurn(true);
	} else
		for (int i = 0; i < num; i++)
			if (!actStep())
				owner_->setDoneTurn(true);
}


// gets the next program that still has moves or actions left in this turn
Program* AICore::getNextUsableProgram() {
	Iterator<Program*> it = owner_->getProgList()->getIterator();
	Program* myProg = NULL;
	while (it.hasNext()) {
		myProg = it.next();
		if (myProg->getMoves() > 0 || myProg->getActionsLeft() > 0)
			break;
		myProg = NULL;
	}
	return myProg;
}

// returns the program belonging to the given player whose core is
// closest to the core of this player's selected program.
// this algorithm takes no regard in terms of occupied or missing tiles.
// if passed NULL as targeted enemy, will check all enemy players
Program* AICore::getNearestEnemyProgramAbsoluteDist(Player* enemy) {
	LinkedList<Program*>* allEnemyProgs = getAllEnemyPrograms();
	LinkedList<Program*>* enemyProgList = NULL;
	if (enemy == NULL)
		enemyProgList = allEnemyProgs;
	else
		enemyProgList = enemy->getProgList();

	Coord nearest = { INT_MAX, INT_MAX };
	Iterator<Program*> enemyProgs = enemyProgList->getIterator();
	while (enemyProgs.hasNext()) {
		Program* enemyProg = enemyProgs.next();
		int newDist = dist(enemyProg->getHead(), owner_->getSelectedProgram()->getHead());
		int oldDist = dist(nearest, owner_->getSelectedProgram()->getHead());
		if (newDist < oldDist)
			nearest = enemyProg->getHead();
	}

	delete allEnemyProgs;
	return owner_->getGame()->getProgramAt(nearest);
}

// returns the program belonging to the given player whose core is
// closest to the core of this player's selected program.
// this algorithm accounts for missing, but not occupied tiles
// if passed NULL as targeted enemy, will check all enemy players
Program* AICore::getNearestEnemyProgramAStarDist(Player* enemy) {
	LinkedList<Program*>* allEnemyProgs = getAllEnemyPrograms();
	LinkedList<Program*>* enemyProgList = NULL;
	if (enemy == NULL)
		enemyProgList = allEnemyProgs;
	else
		enemyProgList = enemy->getProgList();

	Coord nearest = { INT_MAX, INT_MAX };
	Iterator<Program*> enemyProgs = enemyProgList->getIterator();
	while (enemyProgs.hasNext()) {
		Program* enemyProg = enemyProgs.next();
		int newDist = owner_->getSelectedProgramDistAll({ enemyProg->getHead().x, enemyProg->getHead().y });
		int oldDist = owner_->getSelectedProgramDistAll({ nearest.x, nearest.y });
		if (newDist == -1)
			continue;
		else if (newDist < oldDist)
			nearest = enemyProg->getHead();
	}

	delete allEnemyProgs;
	return owner_->getGame()->getProgramAt(nearest);
}

// returns a linked list containing a pointer to every enemy (i.e., not
// on this AI's team) on the grid
LinkedList<Program*>* AICore::getAllEnemyPrograms() {
	LinkedList<Program*>* progs = new LinkedList<Program*>();
	Iterator<Team*> itTeam = owner_->getGame()->getAllTeams()->getIterator();
	while (itTeam.hasNext()) {
		Team* curr = itTeam.next();
		if (curr->getTeamID() != owner_->getTeam()) {
			Iterator<Player*> itPlayer = curr->getAllPlayers()->getIterator();
			while (itPlayer.hasNext()) {
				Player* pCurr = itPlayer.next();
				progs->addAllFirst(pCurr->getProgList());
			}
		}
	}

	//log("found %i enemy progs\n", progs.getLength());
	return progs;
}

Coord* AICore::idealStepTowardsProgram(Program* p) {
	return NULL;
}

Coord* AICore::naiveStepTowardsProgram(Program* p) {
	// create temp variables
	Coord enemyCore = p->getHead();
	Coord myCore = owner_->getSelectedProgram()->getHead();

	// gather the directions it would be optimal for this program to move
	LinkedList<Coord*> idealMoves = LinkedList<Coord*>();
	if (enemyCore.x < myCore.x && owner_->canSelectedProgramMoveBy({ -1, 0 }))
		idealMoves.addFirst(new Coord{ -1, 0 });
	if (enemyCore.x > myCore.x && owner_->canSelectedProgramMoveBy({ 1, 0 }))
		idealMoves.addFirst(new Coord{ 1, 0 });
	if (enemyCore.y < myCore.y && owner_->canSelectedProgramMoveBy({ 0, -1 }))
		idealMoves.addFirst(new Coord{ 0, -1 });
	if (enemyCore.y > myCore.y && owner_->canSelectedProgramMoveBy({ 0, 1 }))
		idealMoves.addFirst(new Coord{ 0, 1 });

	Coord* ret = NULL;

	// if there are no ideal moves
	if (idealMoves.getLength() == 0) {
		// gather marginally acceptable moves
		LinkedList<Coord*> nonidealMoves = LinkedList<Coord*>();
		if (enemyCore.x < myCore.x || enemyCore.x > myCore.x) {
			if (owner_->canSelectedProgramMoveBy({ 0, -1 }))
				nonidealMoves.addFirst(new Coord{ 0, -1 });
			if (owner_->canSelectedProgramMoveBy({ 0, 1 }))
				nonidealMoves.addFirst(new Coord{ 0, 1 });
		}
		if (enemyCore.y < myCore.y || enemyCore.y > myCore.y) {
			if (owner_->canSelectedProgramMoveBy({ -1, 0 }))
				nonidealMoves.addFirst(new Coord{ -1, 0 });
			if (owner_->canSelectedProgramMoveBy({ 1, 0 }))
				nonidealMoves.addFirst(new Coord{ 1, 0 });
		}

		// if there are no marginally accaptable moves, just idle
		if (nonidealMoves.getLength() == 0)
			ret = NULL;
		else // otherwise, pick one of the marginally acceptable moves
		{
			Coord* temp = nonidealMoves.getObjectAt(randInt() % nonidealMoves.getLength());
			ret = new Coord{ temp->x, temp->y };
			while (idealMoves.getLength() > 0) delete idealMoves.poll();
		}
	} else // otherwise, pick one of the ideal moves
	{
		Coord* temp = idealMoves.getObjectAt(randInt() % idealMoves.getLength());
		ret = new Coord{ temp->x, temp->y };
		while (idealMoves.getLength() > 0) delete idealMoves.poll();
	}

	return ret;
}
