#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;
class ProgramActionMirror;

class ProgramMirror {
public:
	ProgramMirror(PROGRAM type, int programID);
	virtual ~ProgramMirror();

	int getColor(int);
	void setColor(int, int, int);
	PROGRAM getType();
	void setType(PROGRAM);

	int getHealth();
	int getMaxHealth();
	void setMaxHealth(int);
	int getMoves();
	void setMoves(int);
	int getMaxMoves();
	void setMaxMoves(int);
	int getActionsLeft();
	void setActionsLeft(int);
	int getMaxActions();
	void setMaxActions(int);

	PlayerMirror* getOwner();
	void setOwner(PlayerMirror*);
	std::string getName();
	void setName(std::string);
	std::string getDescription();
	void setDescription(std::string);
	int getCost();
	void setCost(int);

	void addTail(Coord);
	void addHead(Coord);
	void removeTile(Coord);

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

	int getTeamID();
	int getProgramID();
protected:
private:
	LinkedList<Coord*>* tiles_;

	LinkedList<ProgramActionMirror*>* actionList_;

	PROGRAM type_;
	int maxHealth_;
	int moves_, maxMoves_;
	int actionsLeft_, maxActions_;

	int color_[3];
	std::string name_;
	std::string description_;
	int cost_;

	PlayerMirror* owner_;
	int programID_;
};