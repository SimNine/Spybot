#pragma once

#include "Standard.h"
#include "LinkedList.h"

class ProgramAction;
class Game;
class Program;
class AICore;

class Player {
public:
	Player(Game*, int);
	virtual ~Player();

	void endTurn();
	bool getDoneTurn();
	void setDoneTurn(bool b);

	LinkedList<Program*>* getProgList();
	Program* getProgramByID(int progID);
	int getTeam();

	void addProgram(Program*);

	void setSelectedTile(Coord);
	Coord getSelectedTile();
	void setSelectedAction(ProgramAction*);
	ProgramAction* getSelectedAction();
	void useSelectedActionAt(Coord);
	void setSelectedProgram(Program*);
	Program* getSelectedProgram();
	void moveSelectedProgram(Coord);
	void moveSelectedProgramBy(Coord);
	bool canSelectedProgramMoveTo(Coord);
	bool canSelectedProgramMoveBy(Coord);
	int getSelectedProgramDist(Coord);
	int getSelectedProgramDistAll(Coord);
	int getSelectedActionDist(Coord);
	Coord getFarthestTile(Program*);

	int getPlayerID();
	void setPlayerID(int playerID);

	void setMind(AICore* mind);
	AICore* getMind();

	SDL_Color getColor();
	Game* getGame();
protected:
	Game* game_;
	int team_;
	bool doneTurn_;
	Program* selectedProgram_;
	ProgramAction* selectedAction_;
	LinkedList<Program*>* progsOwned_;

	// helper methods for AI subclasses
	void calculateProgramDist(Program*);
private:
	Coord selectedTile_;
	int selectedProgDist_[200][200];
	int selectedProgDistAll_[200][200];
	int selectedActionDist_[200][200];

	SDL_Color color_;

	int playerID_;

	AICore* brain_;
};