#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;
class ProgramMirror;
class ProgramActionMirror;
class TeamMirror;
class SpawnGroupMirror;

class GameMirror {
public:
	// constructors, destructor
	GameMirror();
	virtual ~GameMirror();

	//getters and setters
	LinkedList<TeamMirror*>* getAllTeams();
	TeamMirror* getTeamByID(int teamNum);
	PlayerMirror* getPlayerByID(int playerID);
	void setCurrTurnPlayer(PlayerMirror* p);
	PlayerMirror* getCurrTurnPlayer();

	int getLeftBound();
	int getRightBound();
	int getTopBound();
	int getBottomBound();

	void setTileAt(Coord, TILE);
	TILE getTileAt(Coord);
	void setItemAt(Coord, ITEM);
	ITEM getItemAt(Coord);

	void setProgramAt(Coord, ProgramMirror*);
	ProgramMirror* getProgramAt(Coord);

	SpawnGroupMirror* getSpawnGroupAt(Coord pos);
	SpawnGroupMirror* getSpawnGroupByID(int groupID);

	bool isOOB(Coord);
	bool isTiled(Coord);

	void useActionAt(PlayerMirror* userPlayer, ProgramMirror* userProgram, ProgramActionMirror* action, Coord coord);

	GAMESTATUS getStatus();
	void setStatus(GAMESTATUS gs);

	TeamMirror* addTeam(int teamID);
	PlayerMirror* addPlayer(int playerID, int teamID);
	ProgramMirror* addProgram(PROGRAM type, int programID, int playerID, int teamID);
	SpawnGroupMirror* addSpawnGroup(int groupID);
	void removeTeam(int teamID);
	void removePlayer(int playerID, int teamID);
	void removeProgram(int programID, int playerID, int teamID);
	void removeSpawnGroup(int groupID);
protected:
private:
	// players
	LinkedList<TeamMirror*>* teamList_;

	// spawn groups
	LinkedList<SpawnGroupMirror*>* spawnGroupList_;

	// effective grid bounds
	int gridLeftBound_, gridRightBound_, gridTopBound_, gridBottomBound_;

	// grid contents
	TILE gridTiles_[200][200];
	ITEM gridItems_[200][200];
	ProgramMirror* gridPrograms_[200][200];

	BACKGROUND bkg_;

	// game status
	GAMESTATUS status_;
	PlayerMirror* currTurnPlayer_;
};