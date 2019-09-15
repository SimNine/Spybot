#ifndef PLAYER_H
#define PLAYER_H

#include "Program.h"
#include "Pair.h"

// avoid circular dependency
class Game;
class Program;

class Player
{
    public:
        Player(Game*, int);
        virtual ~Player();

        void endTurn();
        bool getDoneTurn();

        LinkedList<Program*>* getProgList();
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
    protected:
        Game* game;
        int team;
        bool doneTurn;
        Program* selectedProgram;
        ProgramAction* selectedAction;
        LinkedList<Program*>* progsOwned;

        // helper methods for AI subclasses
        void calculateProgramDist(Program*);
    private:
        Coord selectedTile;
        int selectedProgDist[200][200];
        int selectedProgDistAll[200][200];
        int selectedActionDist[200][200];
};

#endif // PLAYER_H
