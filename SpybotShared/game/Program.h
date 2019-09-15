#ifndef PROGRAM_H
#define PROGRAM_H

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
	LinkedList<Coord*>* tiles;

	LinkedList<ProgramAction*>* actionList;
	int actionsLeft;

	PROGRAM type;
	int maxHealth;
	int moves, maxMoves;
	int color[3];
	int team;
	Player* owner;
	std::string name;
	std::string description;
	int cost;

	int programID_;
};

#endif // PROGRAM_H
