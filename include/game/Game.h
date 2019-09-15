#ifndef GAME_H
#define GAME_H

#include <string>

#include "Program.h"
#include "Pair.h"
#include "Player.h"
#include "AICore.h"

// avoid circular dependency
class AICore;

class Game
{
    public:
        // constructors, destructor
        Game();
        Game(std::string);
        void initBoard();
        virtual ~Game();

        // level save method
        void saveLevel();

        //getters and setters
        Player* getHumanPlayer();
        LinkedList<AICore*>* getAIPlayers();

        int getLeftBound();
        int getRightBound();
        int getTopBound();
        int getBottomBound();

        void setTileAt(Coord, TILE);
        TILE getTileAt(Coord);
        void setProgramAt(Coord, Program*);
        Program* getProgramAt(Coord);
        void setItemAt(Coord, ITEM);
        ITEM getItemAt(Coord);
        bool isOOB(Coord);
        bool isTiled(Coord);

        void setBackground(BACKGROUND);
        BACKGROUND getBackground();
    protected:
    private:
        // players
        Player* playerHuman;
        LinkedList<AICore*>* playerCompList;

        // effective grid bounds
        int gridLeftBound;
        int gridRightBound;
        int gridTopBound;
        int gridBottomBound;

        // grid contents
        TILE gridTiles[200][200];
        ITEM gridItems[200][200];
        Program* gridPrograms[200][200];
        void removeReferencesToProgram(Program*);
        BACKGROUND bkg;

        // helper methods
        void loadLevel(std::string);
        void drawRectInBoard(TILE, Coord, Coord);
        void fillRectInBoard(TILE, Coord, Coord);
        void drawOvalInBoard(TILE, Coord, Coord);
        void fillOvalInBoard(TILE, Coord, Coord);
        bool isDrawValid(Coord, Coord);
};

#endif // GAME_H
