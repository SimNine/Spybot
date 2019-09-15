#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;
class ProgramAction;

class Program {
public:
	Program(PROGRAM, int, Coord);
	virtual ~Program();

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
	Player* getOwner();
	void setOwner(Player*);
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
	void addAction(ProgramAction*);
	LinkedList<ProgramAction*>* getActions();
	ProgramAction* getActionByID(int actionID);

	int getProgramID();
	void setProgramID(int progID);
protected:
private:
	LinkedList<Coord*>* tiles_;

	LinkedList<ProgramAction*>* actionList_;
	int actionsLeft_;

	PROGRAM type_;
	int maxHealth_;
	int moves_, maxMoves_;
	int color_[3];
	int team_;
	Player* owner_;
	std::string name_;
	std::string description_;
	int cost_;

	int programID_;
};