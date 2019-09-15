#ifndef PROGRAM_H
#define PROGRAM_H

#include <SDL.h>

#include "LinkedList.h"
#include "Enums.h"
#include "ProgramAction.h"
#include "Pair.h"
#include "Coord.h"
#include "Player.h"

class Player;

class Program
{
    public:
        Program(PROGRAM, int, Coord);
        virtual ~Program();

        SDL_Texture* getIcon();
        void setIcon(SDL_Texture*);
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

        void endTurn();
        bool isDone();

        void addAction(MOVEPRESET);
        void addAction(ProgramAction*);
        LinkedList<ProgramAction*>* getActions();
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
        SDL_Texture* icon;
        std::string name;
        std::string description;
        int cost;
};

#endif // PROGRAM_H
