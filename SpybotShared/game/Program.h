#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;
class ProgramAction;

class Program {
public:
	Program(PROGRAM type);
	virtual ~Program();

	int getColor(int);
	void setColor(int r, int g, int b);
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

	Player* getOwner();
	void setOwner(Player*);
	int getTeam();
	std::string getName();
	void setName(std::string);
	std::string getDescription();
	void setDescription(std::string);
	int getCost();
	void setCost(int);

	void moveTo(Coord);

	void addHead(Coord);
	void addTail(Coord);
	void removeTile(Coord);

	Coord getHead();
	Coord getTail();
	LinkedList<Coord*>* getTiles();

	void endTurn();
	bool isDone();

	void addAction(ACTION);
	void addAction(ProgramAction*);
	LinkedList<ProgramAction*>* getActions();
	ProgramAction* getActionByID(int actionID);

	int getProgramID();
protected:
private:
	PROGRAM type_;

	LinkedList<Coord*>* tiles_;

	LinkedList<ProgramAction*>* actionList_;

	int maxHealth_;
	int moves_, maxMoves_;
	int actionsLeft_, maxActions_;

	int color_[3];
	std::string name_;
	std::string description_;
	int cost_;

	Player* owner_;
	int programID_;
};