#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;
class Program;
class Team;

class Game {
public:
	// constructors, destructor
	Game(bool serverSide);
	Game(bool serverSide, std::string levelString);
	void initBoard();
	virtual ~Game();

	// level save method
	void saveLevel(std::string fileName);

	//getters and setters
	LinkedList<Team*>* getAllTeams();
	Team* getTeamByNum(int teamNum);
	Player* getPlayerByID(int playerID);
	Player* getFollowingPlayer(Player* currPlayer);
	void setCurrTurnPlayer(Player* p);
	Player* getCurrTurnPlayer();

	void checkForWinCondition();

	int getLeftBound();
	int getRightBound();
	int getTopBound();
	int getBottomBound();

	void setTileAt(Coord, TILE);
	TILE getTileAt(Coord);
	void setProgramAt(Coord, Program*);
	Program* getProgramAt(Coord);
	void moveProgramTo(Program*, Coord);
	void setItemAt(Coord, ITEM);
	ITEM getItemAt(Coord);
	bool isOOB(Coord);
	bool isTiled(Coord);

	void setBackground(BACKGROUND);
	BACKGROUND getBackground();

	GAMESTATUS getStatus();
	void setStatus(GAMESTATUS gs);

	bool isServerSide();
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
	void removeReferencesToProgram(Program*);
	BACKGROUND bkg_;

	// helper methods
	void loadLevel(std::string);
	void drawRectInBoard(TILE, Coord, Coord);
	void fillRectInBoard(TILE, Coord, Coord);
	void drawOvalInBoard(TILE, Coord, Coord);
	void fillOvalInBoard(TILE, Coord, Coord);
	bool isDrawValid(Coord, Coord);

	// game status
	GAMESTATUS status_;
	Player* currTurnPlayer_;

	// boolean indicating whether this game is serverside (master) or clientside (slave)
	bool serverSide_;
};