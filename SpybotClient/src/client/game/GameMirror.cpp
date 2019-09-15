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
#include "SpawnGroupMirror.h"

GameMirror::GameMirror() {
	teamList_ = new LinkedList<TeamMirror*>();
	spawnGroupList_ = new LinkedList<SpawnGroupMirror*>();
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

TILE GameMirror::getTileAt(Coord pos) {
	if (pos.x >= 200 || pos.x < 0 || pos.y >= 200 || pos.y < 0)
		return TILE_NONE;
	return gridTiles_[pos.x][pos.y];
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

ProgramMirror* GameMirror::getProgramAt(Coord pos) {
	return gridPrograms_[pos.x][pos.y];
}

void GameMirror::setItemAt(Coord pos, ITEM i) {
	gridItems_[pos.x][pos.y] = i;
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

PlayerMirror* GameMirror::getCurrTurnPlayer() {
	return currTurnPlayer_;
}

void GameMirror::setCurrTurnPlayer(PlayerMirror* p) {
	currTurnPlayer_ = p;
}

LinkedList<TeamMirror*>* GameMirror::getAllTeams() {
	return teamList_;
}

TeamMirror* GameMirror::getTeamByID(int teamNum) {
	Iterator<TeamMirror*> it = teamList_->getIterator();
	while (it.hasNext()) {
		TeamMirror* curr = it.next();
		if (curr->getTeamID() == teamNum)
			return curr;
	}

	return NULL;
}

void GameMirror::useActionAt(PlayerMirror* userPlayer, ProgramMirror* userProgram, ProgramActionMirror* action, Coord pos) {
	ProgramMirror* tgtProg = gridPrograms_[pos.x][pos.y];

	switch (action->type_) {
	case ACTIONTYPE_DAMAGE:
		if (tgtProg->getTeamID() != userPlayer->getTeamID()) {
			_gameOverlay->addAnimation(new AnimationAttack(tgtProg->getHead(), action->power_));
			for (int i = 0; i < action->power_; i++) {
				Coord* curr = tgtProg->popTail();
				if (curr == NULL) {
					break;
				} else {
					SDL_Color c = tgtProg->getOwner()->getColor();
					_gameOverlay->addAnimation(new AnimationTileFade(*curr, i * 255 + 255, c.r, c.g, c.b));
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
		log("CLIENT ERR: action type HEAL not implemented yet\n");
	default:
		break;
	}

	userProgram->setActionsLeft(userProgram->getActionsLeft() - 1);
	userPlayer->setSelectedAction(NULL);
	userPlayer->setSelectedProgram(userProgram);
}

TeamMirror* GameMirror::addTeam(int teamID) {
	TeamMirror* t = new TeamMirror(teamID);
	teamList_->addFirst(t);
	return t;
}

PlayerMirror* GameMirror::addPlayer(int playerID, int teamID) {
	TeamMirror* t = getTeamByID(teamID);
	if (t == NULL) {
		log("CLIENT ERR: tried to create PlayerMirror " + to_string(playerID) + " on nonexistent TeamMirror " + to_string(teamID) + "\n");
		return NULL;
	}

	PlayerMirror* p = new PlayerMirror(this, playerID, teamID);
	t->getAllPlayers()->addFirst(p);
	return p;
}

ProgramMirror* GameMirror::addProgram(PROGRAM type, int programID, int playerID, int teamID) {
	TeamMirror* t = getTeamByID(teamID);
	if (t == NULL) {
		log("CLIENT ERR: tried to create new ProgramMirror " + to_string(programID) + " on PlayerMirror " + to_string(playerID) + " on nonexistent TeamMirror " + to_string(teamID) + "\n");
		return NULL;
	}

	PlayerMirror* p = t->getPlayerByID(playerID);
	if (p == NULL) {
		log("CLIENT ERR: tried to create new ProgramMirror " + to_string(programID) + " on nonexistent PlayerMirror " + to_string(playerID) + " on TeamMirror %i\n");
		return NULL;
	}

	ProgramMirror* pr = new ProgramMirror(type, programID);
	p->addProgram(pr);
	return pr;
}

SpawnGroupMirror* GameMirror::addSpawnGroup(int groupID) {
	SpawnGroupMirror* g = new SpawnGroupMirror(groupID);
	spawnGroupList_->addFirst(g);

	return g;
}

void GameMirror::removeTeam(int teamID) {
	TeamMirror* t = getTeamByID(teamID);
	if (t == NULL) {
		log("CLIENT ERR: tried to remove nonexistent team " + to_string(teamID) + "\n");
		return;
	}

	while (t->getAllPlayers()->getLength() > 0) {
		removePlayer(t->getAllPlayers()->getFirst()->getPlayerID(), teamID);
	}

	teamList_->remove(t);
	delete t;
}

void GameMirror::removePlayer(int playerID, int teamID) {
	TeamMirror* t = getTeamByID(teamID);
	if (t == NULL) {
		log("CLIENT ERR: tried to remove player " + to_string(playerID) + " on nonexistent team " + to_string(teamID) + "\n");
		return;
	}

	PlayerMirror* p = t->getPlayerByID(playerID);
	if (p == NULL) {
		log("CLIENT ERR: tried to remove nonexistent player " + to_string(playerID) + " on team " + to_string(teamID) + "\n");
		return;
	}

	while (p->getProgList()->getLength() > 0) {
		removeProgram(p->getProgList()->getFirst()->getProgramID(), p->getPlayerID(), t->getTeamID());
	}

	t->getAllPlayers()->remove(p);
	delete p;
}

void GameMirror::removeProgram(int programID, int playerID, int teamID) {
	TeamMirror* t = getTeamByID(teamID);
	if (t == NULL) {
		log("CLIENT ERR: tried to remove program mirror " + to_string(programID) + " on player " + to_string(playerID) + " on nonexistent team " + to_string(teamID) + "\n");
		return;
	}

	PlayerMirror* p = t->getPlayerByID(playerID);
	if (p == NULL) {
		log("CLIENT ERR: tried to remove program mirror " + to_string(programID) + " on nonexistent player " + to_string(playerID) + " on team " + to_string(teamID) + "\n");
		return;
	}

	ProgramMirror* pr = p->getProgramByID(programID);
	if (pr == NULL) {
		log("CLIENT ERR: tried to remove nonexistent program mirror " + to_string(programID) + " on player " + to_string(playerID) + " on team " + to_string(teamID) + "\n");
		return;
	}

	while (pr->getTiles()->getLength() > 0) {
		pr->removeTile(pr->getTail());
	}

	p->getProgList()->remove(pr);
	delete pr;
}

void GameMirror::removeSpawnGroup(int groupID) {
	SpawnGroupMirror* g = NULL;
	Iterator<SpawnGroupMirror*> it = spawnGroupList_->getIterator();
	while (it.hasNext()) {
		SpawnGroupMirror* curr = it.next();
		if (curr->getGroupID() == groupID) {
			g = curr;
			break;
		}
	}

	if (g == NULL) {
		log("CLIENT ERR: tried to remove nonexistent spawn group mirror " + to_string(groupID) + "\n");
		return;
	}

	while (g->getTiles()->getLength() > 0)
		g->removeTile(*g->getTiles()->getFirst());

	spawnGroupList_->remove(g);
	delete g;
}

SpawnGroupMirror* GameMirror::getSpawnGroupByID(int groupID) {
	Iterator<SpawnGroupMirror*> it = spawnGroupList_->getIterator();
	while (it.hasNext()) {
		SpawnGroupMirror* curr = it.next();
		if (curr->getGroupID() == groupID) {
			return curr;
		}
	}

	return NULL;
}

SpawnGroupMirror* GameMirror::getSpawnGroupAt(Coord pos) {
	Iterator<SpawnGroupMirror*> it = spawnGroupList_->getIterator();
	while (it.hasNext()) {
		SpawnGroupMirror* curr = it.next();
		if (curr->containsTile(pos)) {
			return curr;
		}
	}

	return NULL;
}