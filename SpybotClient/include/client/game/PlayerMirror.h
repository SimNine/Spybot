#pragma once

#include "Standard.h"
#include "LinkedList.h"

class ProgramActionMirror;
class GameMirror;
class ProgramMirror;

class PlayerMirror {
public:
	PlayerMirror(GameMirror*, int);
	virtual ~PlayerMirror();

	void endTurn();

	LinkedList<ProgramMirror*>* getProgList();
	ProgramMirror* getProgramByID(int progID);
	int getTeam();

	void addProgram(ProgramMirror*);

	void setSelectedTile(Coord);
	Coord getSelectedTile();
	void setSelectedAction(ProgramActionMirror*);
	ProgramActionMirror* getSelectedAction();
	void setSelectedProgram(ProgramMirror*);
	ProgramMirror* getSelectedProgram();
	bool canSelectedProgramMoveTo(Coord);
	int getSelectedProgramDist(Coord);
	int getSelectedProgramDistAll(Coord);
	int getSelectedActionDist(Coord);

	int getPlayerID();
	void setPlayerID(int playerID);

	SDL_Color getColor();
	GameMirror* getGame();
protected:
	GameMirror* game_;
	int team_;
	ProgramMirror* selectedProgram_;
	ProgramActionMirror* selectedAction_;
	LinkedList<ProgramMirror*>* progsOwned_;

	// helper methods for AI subclasses
	void calculateProgramDist(ProgramMirror*);
private:
	Coord selectedTile_;
	int selectedProgDist_[200][200];
	int selectedProgDistAll_[200][200];
	int selectedActionDist_[200][200];

	SDL_Color color_;

	int playerID_;
};