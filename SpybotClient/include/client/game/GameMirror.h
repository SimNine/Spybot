#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;
class ProgramMirror;
class ProgramActionMirror;
class TeamMirror;

class GameMirror {
public:
	// constructors, destructor
	GameMirror();
	virtual ~GameMirror();

	//getters and setters
	LinkedList<TeamMirror*>* getAllTeams();
	TeamMirror* getTeamByNum(int teamNum);
	PlayerMirror* getPlayerByID(int playerID);
	void setCurrTurnPlayer(PlayerMirror* p);
	PlayerMirror* getCurrTurnPlayer();

	int getLeftBound();
	int getRightBound();
	int getTopBound();
	int getBottomBound();

	void setTileAt(Coord, TILE);
	TILE getTileAt(Coord);
	void setProgramAt(Coord, ProgramMirror*);
	ProgramMirror* getProgramAt(Coord);
	void moveProgramTo(ProgramMirror*, Coord);
	void setItemAt(Coord, ITEM);
	ITEM getItemAt(Coord);
	bool isOOB(Coord);
	bool isTiled(Coord);

	void useActionAt(PlayerMirror* userPlayer, ProgramMirror* userProgram, ProgramActionMirror* action, Coord coord);

	GAMESTATUS getStatus();
	void setStatus(GAMESTATUS gs);
protected:
private:
	// players
	LinkedList<TeamMirror*>* teamList_;

	// effective grid bounds
	int gridLeftBound_, gridRightBound_, gridTopBound_, gridBottomBound_;

	// grid contents
	TILE gridTiles_[200][200];
	ITEM gridItems_[200][200];
	ProgramMirror* gridPrograms_[200][200];
	void removeReferencesToProgram(ProgramMirror* p);
	BACKGROUND bkg_;

	// game status
	GAMESTATUS status_;
	PlayerMirror* currTurnPlayer_;
};