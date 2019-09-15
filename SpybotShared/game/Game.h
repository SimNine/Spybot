#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;
class Program;
class Team;

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
	void removeTeam(int teamID);
	void removePlayer(int playerID, int teamID);
	void removeProgram(int programID, int playerID, int teamID);

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

	void setTileAt(Coord, TILE);
	TILE getTileAt(Coord);
	void setItemAt(Coord, ITEM);
	ITEM getItemAt(Coord);

	// ONLY call this method from the Player class
	void setProgramAt(Coord, Program*);
	Program* getProgramAt(Coord);

	bool isOOB(Coord);
	bool isTiled(Coord);
	void setSpawnGroup(Coord pos, int group);

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

	// effective grid bounds
	int gridLeftBound_, gridRightBound_, gridTopBound_, gridBottomBound_;

	// grid contents
	TILE gridTiles_[200][200];
	ITEM gridItems_[200][200];
	Program* gridPrograms_[200][200];
	int gridSpawnGroups_[200][200];

	BACKGROUND bkg_;

	// helper methods
	void drawRectInBoard(TILE, Coord, Coord);
	void fillRectInBoard(TILE, Coord, Coord);
	void drawOvalInBoard(TILE, Coord, Coord);
	void fillOvalInBoard(TILE, Coord, Coord);
	bool isDrawValid(Coord, Coord);

	// game status
	GAMESTATUS status_;
	Player* currTurnPlayer_;
};