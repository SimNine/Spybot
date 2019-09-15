#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;
class Program;
class Team;
class SpawnGroup;

class Game {
public:
	// constructors, destructor
	Game(std::string levelString);
	void initBoard();
	virtual ~Game();

	// entity creation and deletion
	Team* addTeam();
	Player* addPlayer(int teamID);
	Program* addProgram(PROGRAM type, int playerID, int teamID);
	SpawnGroup* addSpawnGroup();
	void removeTeam(int teamID);
	void removePlayer(int playerID, int teamID);
	void removeProgram(int programID, int playerID, int teamID);
	void removeSpawnGroup(int groupID);

	// getters and setters
	LinkedList<Team*>* getAllTeams();
	Team* getTeamByID(int teamNum);
	Team* getDefaultTeamAI();
	Team* getDefaultTeamHuman();

	Player* getPlayerByID(int playerID);
	Player* getFollowingPlayer(Player* currPlayer);
	Player* getCurrTurnPlayer();
	void setCurrTurnPlayer(Player* p);

	void checkForWinCondition();

	int getLeftBound();
	int getRightBound();
	int getTopBound();
	int getBottomBound();

	void setTileAt(Coord pos, TILE tiletype);
	TILE getTileAt(Coord pos);
	void drawRectInBoard(TILE type, Coord pos, Coord dims);
	void fillRectInBoard(TILE type, Coord pos, Coord dims);
	void drawOvalInBoard(TILE type, Coord pos, Coord dims);
	void fillOvalInBoard(TILE type, Coord pos, Coord dims);

	void setItemAt(Coord, ITEM);
	ITEM getItemAt(Coord);

	// ONLY call this method from the Player class
	void setProgramAt(Coord, Program*);
	Program* getProgramAt(Coord);

	bool isOOB(Coord);
	bool isTiled(Coord);

	SpawnGroup* getUnassignedSpawnGroup();
	SpawnGroup* getSpawnGroupByID(int groupID);
	SpawnGroup* getSpawnGroupAt(Coord pos);
	LinkedList<SpawnGroup*>* getSpawnGroups();

	void setBackground(BACKGROUND);
	BACKGROUND getBackground();

	GAMESTATUS getStatus();
	void setStatus(GAMESTATUS gs);

	void loadLevel(std::string);
	void saveLevel(std::string fileName);
protected:
private:
	// players
	LinkedList<Team*>* teamList_;

	// spawn groups
	LinkedList<SpawnGroup*>* spawnGroupList_;

	// effective grid bounds
	int gridLeftBound_, gridRightBound_, gridTopBound_, gridBottomBound_;

	// grid contents
	TILE gridTiles_[200][200];
	ITEM gridItems_[200][200];
	Program* gridPrograms_[200][200];

	BACKGROUND bkg_;

	// helper method
	bool isDrawValid(Coord, Coord);

	// game status
	GAMESTATUS status_;
	Player* currTurnPlayer_;
};