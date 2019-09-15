#ifndef GAME_H
#define GAME_H

#include "Standard.h"
#include "LinkedList.h"

class Program;
class Player;

class Game
{
    public:
        // constructors, destructor
        Game();
        Game(std::string);
        virtual ~Game();

        // level save method
        void saveLevel();

        //getters and setters
        LinkedList<Player*>* getHumanPlayers();
        LinkedList<Player*>* getAIPlayers();
		Player* getPlayerByID(int playerID);
		void setCurrTurnPlayer(Player* p);
		Player* getCurrTurnPlayer();

        int getLeftBound();
        int getRightBound();
        int getTopBound();
        int getBottomBound();

        void setTileAt(Coord, TILE);
        TILE getTileAt(Coord);
        void setProgramAt(Coord, Program*);
        Program* getProgramAt(Coord);
		void moveProgramTo(Program*, Coord);
        void setItemAt(Coord, ITEM);
        ITEM getItemAt(Coord);
        bool isOOB(Coord);
        bool isTiled(Coord);

        void setBackground(BACKGROUND);
        BACKGROUND getBackground();

		GAMESTATUS getStatus();
		void setStatus(GAMESTATUS gs);
    protected:
    private:
        // players
        LinkedList<Player*>* playerHumanList;
        LinkedList<Player*>* playerCompList;

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
		void initBoard();
        void drawRectInBoard(TILE, Coord, Coord);
        void fillRectInBoard(TILE, Coord, Coord);
        void drawOvalInBoard(TILE, Coord, Coord);
        void fillOvalInBoard(TILE, Coord, Coord);
        bool isDrawValid(Coord, Coord);

		// game state
		GAMESTATUS status_;
		Player* currTurnPlayer;
};

#endif // GAME_H
