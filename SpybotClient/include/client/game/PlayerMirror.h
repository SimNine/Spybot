#pragma once

#include "Standard.h"
#include "LinkedList.h"

class ProgramActionMirror;
class GameMirror;
class ProgramMirror;

class PlayerMirror {
public:
	PlayerMirror(GameMirror* g, int playerID, int teamID);
	virtual ~PlayerMirror();

	void endTurn();

	LinkedList<ProgramMirror*>* getProgList();
	ProgramMirror* getProgramByID(int progID);

	void addProgram(ProgramMirror* p);

	void setSelectedTile(Coord);
	Coord getSelectedTile();
	void setSelectedAction(ProgramActionMirror*);
	ProgramActionMirror* getSelectedAction();
	void setSelectedProgram(ProgramMirror*);
	ProgramMirror* getSelectedProgram();
	bool canSelectedProgramMoveTo(Coord);

	bool getAI();
	void setAI(bool);

	int getSelectedProgramDist(Coord);
	int getSelectedProgramDistAll(Coord);

	int getSelectedActionDist(Coord);
	int getSelectedActionDistAll(Coord);

	int getTeamID();
	int getPlayerID();

	SDL_Color getColor();
	GameMirror* getGame();
private:
	Coord selectedTile_;
	int selectedProgDist_[200][200];
	int selectedProgDistAll_[200][200];
	int selectedActionDist_[200][200];
	int selectedActionDistAll_[200][200];

	GameMirror* game_;
	ProgramMirror* selectedProgram_;
	ProgramActionMirror* selectedAction_;
	LinkedList<ProgramMirror*>* progsOwned_;

	void calculateProgramDist(ProgramMirror*);

	bool isAI_;

	SDL_Color color_;

	int teamID_;
	int playerID_;
};