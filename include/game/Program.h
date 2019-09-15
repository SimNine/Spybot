#ifndef PROGRAM_H
#define PROGRAM_H

#include <SDL.h>

#include "LinkedList.h"
#include "Enums.h"
#include "ProgramAction.h"
#include "Pair.h"

class Program
{
    public:
        Program(PROGRAM, int, int, int);
        virtual ~Program();

        SDL_Texture* getIcon();
        void setIcon(SDL_Texture*);
        int getColor(int);
        void setColor(int, int, int);
        int getCoreX();
        void setCoreX(int);
        int getCoreY();
        void setCoreY(int);
        PROGRAM getType();
        void setType(PROGRAM);
        int getHealth();
        int getMaxHealth();
        void setMaxHealth(int);
        int getMoves();
        void setMoves(int);
        int getMaxMoves();
        void setMaxMoves(int);
        int getTeam();
        void setTeam(int);
        std::string getName();
        void setName(std::string);
        std::string getDescription();
        void setDescription(std::string);
        int getCost();
        void setCost(int);

        void moveTo(int, int);
        void addTail(int, int);
        Pair<int>* getHead();
        Pair<int>* getTail();

        void endTurn();

        void addAction(ProgramAction*);
        LinkedList<ProgramAction*>* getActions();
    protected:
    private:
        LinkedList<Pair<int>*> tiles;

        PROGRAM type;
        int maxHealth;
        int moves, maxMoves;
        int color[3];
        int team;
        LinkedList<ProgramAction*>* actionList;
        SDL_Texture* icon;
        std::string name;
        std::string description;
        int cost;
};

#endif // PROGRAM_H
