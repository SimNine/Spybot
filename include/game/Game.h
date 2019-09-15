#ifndef GAME_H
#define GAME_H

#include <string>

#include "Program.h"
#include "Pair.h"

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

        // gameplay methods
        void endTurn();

        //getters and setters
        int getLeftBound();
        int getRightBound();
        int getTopBound();
        int getBottomBound();

        void setTileAt(int, int, TILE);
        TILE getTileAt(int, int);
        void setProgramAt(int, int, Program*);
        Program* getProgramAt(int, int);
        void setItemAt(int, int, ITEM);
        ITEM getItemAt(int, int);
        void setBackground(BACKGROUND);
        BACKGROUND getBackground();

        void setSelectedProgram(int, int);
        void moveSelectedProgram(int, int);
        Program* getSelectedProgram();
        int getSelectedProgramDist(int, int);
        Pair<int>* getFarthestTile(Program*);
        Pair<int>* getFarthestContiguousTile(Program*);
    protected:
    private:
        // farthest placed tiles
        int gridLeftBound;
        int gridRightBound;
        int gridTopBound;
        int gridBottomBound;

        // information about the game
        TILE gridTiles[200][200];
        ITEM gridItems[200][200];
        Program* gridPrograms[200][200];
        BACKGROUND bkg;

        // program management
        void deleteProgram(Program*);
        void calculateProgramDist(Program*);

        // working information
        Program* selectedProgram;
        int gridProgramDist[200][200];
        bool gridCheckedTile[200][200];

        // helper methods
        void loadLevel(std::string);
        void drawRectInBoard(TILE, int, int, int, int);
        void fillRectInBoard(TILE, int, int, int, int);
        void drawOvalInBoard(TILE, int, int, int, int);
        void fillOvalInBoard(TILE, int, int, int, int);
        bool isDrawValid(int, int, int, int);
        bool isOOB(int, int);
        bool isTiled(int, int);
};

#endif // GAME_H
