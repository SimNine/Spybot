#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;
class ProgramActionMirror;

class ProgramMirror {
public:
	ProgramMirror(PROGRAM, int, Coord);
	virtual ~ProgramMirror();

	int getColor(int);
	void setColor(int, int, int);
	Coord getCore();
	void setCore(Coord);
	PROGRAM getType();
	void setType(PROGRAM);
	int getHealth();
	int getMaxHealth();
	void setMaxHealth(int);
	int getMoves();
	void setMoves(int);
	int getMaxMoves();
	void setMaxMoves(int);
	PlayerMirror* getOwner();
	void setOwner(PlayerMirror*);
	int getTeam();
	std::string getName();
	void setName(std::string);
	std::string getDescription();
	void setDescription(std::string);
	int getCost();
	void setCost(int);
	int getActionsLeft();
	void setActionsLeft(int);

	void moveTo(Coord);
	void addTail(Coord);
	Coord getHead();
	Coord getTail();
	Coord* popTail();
	LinkedList<Coord*>* getTiles();

	void endTurn();
	bool isDone();

	void addAction(MOVEPRESET);
	void addAction(ProgramActionMirror*);
	LinkedList<ProgramActionMirror*>* getActions();
	ProgramActionMirror* getActionByID(int actionID);

	int getProgramID();
	void setProgramID(int progID);
protected:
private:
	LinkedList<Coord*>* tiles_;

	LinkedList<ProgramActionMirror*>* actionList_;
	int actionsLeft_;

	PROGRAM type_;
	int maxHealth_;
	int moves_, maxMoves_;
	int color_[3];
	int team_;
	PlayerMirror* owner_;
	std::string name_;
	std::string description_;
	int cost_;

	int programID_;
};