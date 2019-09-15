#include "Standard.h"
#include "GameMirror.h"

#include "Global.h"
#include "MiscUtil.h"
#include "PlayerMirror.h"
#include "ProgramMirror.h"
#include "ProgramActionMirror.h"
#include "TeamMirror.h"
#include "GameOverlay.h"
#include "AnimationAttack.h"
#include "AnimationTileFade.h"

GameMirror::GameMirror() {
	teamList_ = new LinkedList<TeamMirror*>();
	status_ = GAMESTATUS_PREGAME;

	for (int x = 0; x < 200; x++) {
		for (int y = 0; y < 200; y++) {
			gridTiles_[x][y] = TILE_NONE;
			gridItems_[x][y] = ITEM_NONE;
			gridPrograms_[x][y] = NULL;
		}
	}

	gridLeftBound_ = 100;
	gridRightBound_ = 100;
	gridTopBound_ = 100;
	gridBottomBound_ = 100;
}

GameMirror::~GameMirror() {
	while (teamList_->getLength() > 0)
		delete teamList_->poll();
	delete teamList_;
}

void GameMirror::setTileAt(Coord pos, TILE t) {
	// set the tile
	gridTiles_[pos.x][pos.y] = t;

	// try increasing bounds
	if (t != TILE_NONE) {
		if (pos.x < gridLeftBound_)
			gridLeftBound_ = pos.x;
		else if (pos.x + 1 > gridRightBound_)
			gridRightBound_ = pos.x + 1;

		if (pos.y < gridTopBound_)
			gridTopBound_ = pos.y;
		else if (pos.y + 1 > gridBottomBound_)
			gridBottomBound_ = pos.y + 1;
	} else { // try decreasing bounds
		if (pos.x == gridLeftBound_) {
			bool b = true;
			while (b && gridLeftBound_ < 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[gridLeftBound_][i] != TILE_NONE)
						b = false;
				if (b)
					gridLeftBound_++;
			}
		} else if (pos.x + 1 == gridRightBound_) {
			bool b = true;
			while (b && gridRightBound_ > 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[gridRightBound_ - 1][i] != TILE_NONE)
						b = false;
				if (b)
					gridRightBound_--;
			}
		}

		if (pos.y == gridTopBound_) {
			bool b = true;
			while (b && gridTopBound_ < 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[i][gridTopBound_] != TILE_NONE)
						b = false;
				if (b)
					gridTopBound_++;
			}
		} else if (pos.y + 1 == gridBottomBound_) {
			bool b = true;
			while (b && gridBottomBound_ > 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[i][gridBottomBound_ - 1] != TILE_NONE)
						b = false;
				if (b)
					gridBottomBound_--;
			}
		}
	}
}

void GameMirror::removeReferencesToProgram(ProgramMirror* p) {
	if (p == NULL)
		return;

	for (int i = 0; i < 200; i++)
		for (int j = 0; j < 200; j++)
			if (gridPrograms_[i][j] == p)
				gridPrograms_[i][j] = NULL;
}

int GameMirror::getLeftBound() {
	return gridLeftBound_;
}

int GameMirror::getRightBound() {
	return gridRightBound_;
}

int GameMirror::getTopBound() {
	return gridTopBound_;
}

int GameMirror::getBottomBound() {
	return gridBottomBound_;
}

void GameMirror::setProgramAt(Coord pos, ProgramMirror* p) {
	gridPrograms_[pos.x][pos.y] = p;
}

void GameMirror::setItemAt(Coord pos, ITEM i) {
	gridItems_[pos.x][pos.y] = i;
}

TILE GameMirror::getTileAt(Coord pos) {
	if (pos.x >= 200 || pos.x < 0 || pos.y >= 200 || pos.y < 0)
		return TILE_NONE;
	return gridTiles_[pos.x][pos.y];
}

ProgramMirror* GameMirror::getProgramAt(Coord pos) {
	return gridPrograms_[pos.x][pos.y];
}

ITEM GameMirror::getItemAt(Coord pos) {
	return gridItems_[pos.x][pos.y];
}

bool GameMirror::isOOB(Coord pos) {
	if (pos.x < 0 || pos.x >= 200 || pos.y < 0 || pos.y >= 200)
		return true;
	else
		return false;
}

bool GameMirror::isTiled(Coord pos) {
	if (isOOB(pos) || gridTiles_[pos.x][pos.y] == TILE_NONE)
		return false;
	else
		return true;
}

PlayerMirror* GameMirror::getPlayerByID(int playerID) {
	Iterator<TeamMirror*> itTeams = teamList_->getIterator();
	while (itTeams.hasNext()) {
		TeamMirror* currTeam = itTeams.next();
		PlayerMirror* p = currTeam->getPlayerByID(playerID);
		if (p != NULL)
			return p;
	}

	return NULL;
}

GAMESTATUS GameMirror::getStatus() {
	return status_;
}

void GameMirror::setStatus(GAMESTATUS g) {
	status_ = g;
}

void GameMirror::moveProgramTo(ProgramMirror* p, Coord c) {
	// delete the tail of this program if it's at max health
	if (p->getHealth() == p->getMaxHealth() && getProgramAt(c) != p) {
		Coord temp = p->getTail();
		gridPrograms_[temp.x][temp.y] = NULL;
	}

	// move the program
	gridPrograms_[c.x][c.y] = p;
	p->moveTo(c);
}

PlayerMirror* GameMirror::getCurrTurnPlayer() {
	return currTurnPlayer_;
}

void GameMirror::setCurrTurnPlayer(PlayerMirror* p) {
	currTurnPlayer_ = p;
}

LinkedList<TeamMirror*>* GameMirror::getAllTeams() {
	return teamList_;
}

TeamMirror* GameMirror::getTeamByNum(int teamNum) {
	Iterator<TeamMirror*> it = teamList_->getIterator();
	while (it.hasNext()) {
		TeamMirror* curr = it.next();
		if (curr->getTeamNum() == teamNum)
			return curr;
	}

	return NULL;
}

void GameMirror::useActionAt(PlayerMirror* userPlayer, ProgramMirror* userProgram, ProgramActionMirror* action, Coord pos) {
	ProgramMirror* tgtProg = gridPrograms_[pos.x][pos.y];

	switch (action->type_) {
	case ACTIONTYPE_DAMAGE:
		if (tgtProg->getTeam() != userPlayer->getTeam()) {
			_gameOverlay->addAnimation(new AnimationAttack(tgtProg->getHead(), action->power_));
			for (int i = 0; i < action->power_; i++) {
				Coord* curr = tgtProg->popTail();
				if (curr == NULL) {
					break;
				} else {
					SDL_Color c = tgtProg->getOwner()->getColor();
					_gameOverlay->addAnimation(new AnimationTileFade(*curr, i * 255 + 255, c.r, c.g, c.b));
					gridPrograms_[curr->x][curr->y] = NULL;
				}
			}
		}

		if (tgtProg->getHealth() <= 0) {
			PlayerMirror* owner = tgtProg->getOwner();
			owner->getProgList()->remove(tgtProg);
			delete tgtProg;
		}
		break;
	case ACTIONTYPE_SPEEDDOWN:
		if (tgtProg->getMaxMoves() < action->power_) {
			tgtProg->setMaxMoves(0);
		} else {
			tgtProg->setMaxMoves(tgtProg->getMaxMoves() - action->power_);
		}

		if (tgtProg->getMoves() > tgtProg->getMaxMoves())
			tgtProg->setMoves(tgtProg->getMaxMoves());
		break;
	case ACTIONTYPE_SPEEDUP:
		tgtProg->setMaxMoves(tgtProg->getMaxMoves() + action->power_);
		tgtProg->setMoves(tgtProg->getMoves() + action->power_);
		break;
	case ACTIONTYPE_TILEDELETE:
		setTileAt(pos, TILE_NONE);
		break;
	case ACTIONTYPE_TILEPLACE:
		setTileAt(pos, TILE_PLAIN);
		break;
	case ACTIONTYPE_MAXHEALTHDOWN:
		if (tgtProg->getMaxHealth() < action->power_ - 1)
			tgtProg->setMaxHealth(0);
		else
			tgtProg->setMaxHealth(tgtProg->getMaxHealth() - action->power_);
		break;
	case ACTIONTYPE_MAXHEALTHUP:
		tgtProg->setMaxHealth(tgtProg->getMaxHealth() + action->power_);
		break;
	case ACTIONTYPE_HEAL:
		printf("CLIENT ERR: action type HEAL not implemented yet\n");
	default:
		break;
	}

	userProgram->setActionsLeft(userProgram->getActionsLeft() - 1);
	userPlayer->setSelectedAction(NULL);
	userPlayer->setSelectedProgram(userProgram);
}