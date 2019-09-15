#include "Standard.h"
#include "Game.h"

#include "Global.h"
#include "Program.h"
#include "Player.h"
#include "Message.h"
#include "Main.h"
#include "Server.h"

Game::Game()
{
    playerCompList = new LinkedList<Player*>();
    playerHumanList = new LinkedList<Player*>();
    initBoard();
}

Game::Game(std::string lvlStr)
{
	playerCompList = new LinkedList<Player*>();
	playerHumanList = new LinkedList<Player*>();
    initBoard();
    loadLevel(lvlStr);
}

Game::~Game()
{
    while (playerHumanList->getLength() > 0)
        delete playerHumanList->poll();
    delete playerHumanList;
    while (playerCompList->getLength() > 0)
        delete playerCompList->poll();
    delete playerCompList;
}

void Game::initBoard()
{
    // initialize the board
    for (int x = 0; x < 200; x++)
    {
        for (int y = 0; y < 200; y++)
        {
            gridTiles[x][y] = TILE_NONE;
            gridItems[x][y] = ITEM_NONE;
            gridPrograms[x][y] = NULL;
        }
    }

    gridLeftBound = 100;
    gridRightBound = 100;
    gridTopBound = 100;
    gridBottomBound = 100;
}

void Game::setTileAt(Coord pos, TILE t)
{
    // check for OOB
    if (isOOB(pos))
        return;

    // set the tile
    gridTiles[pos.x][pos.y] = t;

    // try increasing bounds
    if (t != TILE_NONE)
    {
        if (pos.x < gridLeftBound)
            gridLeftBound = pos.x;
        else if (pos.x + 1 > gridRightBound)
            gridRightBound = pos.x + 1;

        if (pos.y < gridTopBound)
            gridTopBound = pos.y;
        else if (pos.y + 1 > gridBottomBound)
            gridBottomBound = pos.y + 1;
    }
    else // try decreasing bounds
    {
        if (pos.x == gridLeftBound)
        {
            bool b = true;
            while (b && gridLeftBound < 100)
            {
                for (int i = 0; i < 200; i++)
                    if (gridTiles[gridLeftBound][i] != TILE_NONE)
                        b = false;
                if (b)
                    gridLeftBound++;
            }
        }
        else if (pos.x + 1 == gridRightBound)
        {
            bool b = true;
            while (b && gridRightBound > 100)
            {
                for (int i = 0; i < 200; i++)
                    if (gridTiles[gridRightBound - 1][i] != TILE_NONE)
                        b = false;
                if (b)
                    gridRightBound--;
            }
        }

        if (pos.y == gridTopBound)
        {
            bool b = true;
            while (b && gridTopBound < 100)
            {
                for (int i = 0; i < 200; i++)
                    if (gridTiles[i][gridTopBound] != TILE_NONE)
                        b = false;
                if (b)
                    gridTopBound++;
            }
        }
        else if (pos.y + 1 == gridBottomBound)
        {
            bool b = true;
            while (b && gridBottomBound > 100)
            {
                for (int i = 0; i < 200; i++)
                    if (gridTiles[i][gridBottomBound - 1] != TILE_NONE)
                        b = false;
                if (b)
                    gridBottomBound--;
            }
        }
    }
}

bool Game::isDrawValid(Coord pos, Coord dims)
{
    if (pos.x < 0 || pos.x + dims.x > 200 ||
        pos.y < 0 || pos.y + dims.y > 200)
        return false;
    else
        return true;
}

void Game::drawRectInBoard(TILE tileType, Coord pos, Coord dims)
{
    if (!isDrawValid(pos, dims))
        return;

    for (int xx = pos.x; xx < pos.x + dims.x; xx++)
    {
        setTileAt({xx, pos.y}, tileType);
        setTileAt({xx, pos.y + dims.y - 1}, tileType);
    }
    for (int yy = pos.y; yy < pos.y + dims.y; yy++)
    {
        setTileAt({pos.x, yy}, tileType);
        setTileAt({pos.x + dims.x - 1, yy}, tileType);
    }
}

void Game::fillRectInBoard(TILE tileType, Coord pos, Coord dims)
{
    if (!isDrawValid(pos, dims)) return;

    for (int i = pos.x; i < pos.x + dims.x; i++)
        for (int j = pos.y; j < pos.y + dims.y; j++)
            setTileAt({i, j}, tileType);
}

void Game::drawOvalInBoard(TILE tileType, Coord pos, Coord dims)
{

}

void Game::fillOvalInBoard(TILE tileType, Coord pos, Coord dims)
{
    Coord center = {pos.x + dims.x/2, pos.y + dims.y/2};
    for (int i = pos.x; i < pos.x + dims.x - 1; i++)
        for (int j = pos.y; j < pos.y + dims.y - 1; j++)
            if (((i - center.x)*(i - center.x))/((dims.x/2)*(dims.x/2)) &&
                ((j - center.y)*(j - center.y))/((dims.y/2)*(dims.y/2)))
                setTileAt({i, j}, tileType);
}

void Game::saveLevel()
{
    std::ofstream lvl;
    lvl.open("levels/default.urf", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!lvl.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err opening file for saving\n");
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("saving level...\n");

        // begin by writing the sizes of various data types
        int8_t sizeOfInt = sizeof(int);
        int8_t sizeOfChar = sizeof(char);
        int8_t sizeOfDouble = sizeof(double);
        int8_t sizeOfBool = sizeof(bool);
        if (debug >= DEBUG_NORMAL) printf("saving constants... int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);
        lvl.write((char*) &sizeOfInt, 1);
        lvl.write((char*) &sizeOfChar, 1);
        lvl.write((char*) &sizeOfDouble, 1);
        lvl.write((char*) &sizeOfBool, 1);

        // write the size of the game grid to the file
        if (debug >= DEBUG_NORMAL) printf("saving grid bounds... left:%i, right:%i, top:%i, bottom:%i\n", gridLeftBound, gridRightBound, gridTopBound, gridBottomBound);
        lvl.write((char*) &gridLeftBound, sizeOfInt);
        lvl.write((char*) &gridRightBound, sizeOfInt);
        lvl.write((char*) &gridTopBound, sizeOfInt);
        lvl.write((char*) &gridBottomBound, sizeOfInt);

        // write the enum of the level's background
        lvl.write((char*) &bkg_, sizeOfInt);

        // collect all the programs in a linked list
        if (debug >= DEBUG_NORMAL) printf("gathering program list...\n");
        LinkedList<Program*> progs = LinkedList<Program*>();
        for (int x = gridLeftBound; x < gridRightBound; x++)
        {
            for (int y = gridTopBound; y < gridBottomBound; y++)
            {
                if (gridPrograms[x][y] == NULL)
                {
                    continue;
                }
                if (progs.contains(gridPrograms[x][y]))
                {
                    if (debug >= DEBUG_NORMAL) printf("saving program\n");
                    progs.addLast(gridPrograms[x][y]);
                }
            }
        }

        // write all programs to the file
        int numPrograms = progs.getLength();
        if (debug >= DEBUG_NORMAL) printf("saving %i programs...\n", numPrograms);
        lvl.write((char*)(&numPrograms), sizeOfInt);
        for (int i = 0; i < progs.getLength(); i++)
        {
            Program* currProg = progs.getObjectAt(i);

            int xHead = currProg->getCore().x;
            lvl.write((char*)(&xHead), sizeOfInt);
            int yHead = currProg->getCore().y;
            lvl.write((char*)(&yHead), sizeOfInt);
            int type = currProg->getType();
            lvl.write((char*)(&type), sizeOfInt);
            int health = currProg->getHealth();
            lvl.write((char*)(&health), sizeOfInt);
            int maxHealth = currProg->getMaxHealth();
            lvl.write((char*)(&maxHealth), sizeOfInt);
            int maxMoves = currProg->getMaxMoves();
            lvl.write((char*)(&maxMoves), sizeOfInt);
            int team = currProg->getTeam();
            lvl.write((char*)(&team), sizeOfInt);
        }

        // write the grid to the file
        if (debug >= DEBUG_NORMAL) printf("saving tiles, items, and program pointers...\n");
        for (int x = gridLeftBound; x < gridRightBound; x++)
        {
            for (int y = gridTopBound; y < gridBottomBound; y++)
            {
                lvl.write((char*)(&(gridTiles[x][y])), sizeOfInt);
                lvl.write((char*)(&(gridItems[x][y])), sizeOfInt);
                int index = progs.getIndexOf(gridPrograms[x][y]);
                lvl.write((char*)(&index), sizeOfInt);
            }
        }

        // flush and close the file
        if (debug >= DEBUG_MINIMAL) printf("flushing and closing save file... ");
        lvl.flush();
        lvl.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void Game::loadLevel(std::string str)
{
    std::ifstream lvl;
    if (str.size() == 0)
    {
        lvl.open("levels/default.urf", std::ios::in | std::ios::binary);
    }
    else
    {
        lvl.open(str, std::ios::in | std::ios::binary);
    }

    if (!lvl.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err opening level %s\n", str.c_str());
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("loading level %s...\n", str.c_str());

        // read the sizes of various data types
        if (debug >= DEBUG_NORMAL) printf("loading constants...\n");
        int8_t sizeOfInt;
        lvl.read((char*) &sizeOfInt, 1);
        int8_t sizeOfChar;
        lvl.read((char*) &sizeOfChar, 1);
        int8_t sizeOfDouble;
        lvl.read((char*) &sizeOfDouble, 1);
        int8_t sizeOfBool;
        lvl.read((char*) &sizeOfBool, 1);

        // load the size of the game grid
        if (debug >= DEBUG_NORMAL) printf("loading grid bounds...\n");
        int left, right, top, bottom;
        lvl.read((char*) &left, sizeOfInt);
        lvl.read((char*) &right, sizeOfInt);
        lvl.read((char*) &top, sizeOfInt);
        lvl.read((char*) &bottom, sizeOfInt);

        // load the enum of the level's background
        lvl.read((char*) &bkg_, sizeOfInt);
        setBackground(bkg_);

        // load the list of programs
        int numPrograms;
        lvl.read((char*)(&numPrograms), sizeOfInt);
        if (debug >= DEBUG_NORMAL) printf("loading %i programs...\n", numPrograms);
        LinkedList<Program*> progs = LinkedList<Program*>();
        for (int i = 0; i < numPrograms; i++)
        {
            int xHead, yHead, type, health, maxHealth, maxMoves, team;
            lvl.read((char*)(&xHead), sizeOfInt);
            lvl.read((char*)(&yHead), sizeOfInt);
            lvl.read((char*)(&type), sizeOfInt);
            lvl.read((char*)(&health), sizeOfInt);
            lvl.read((char*)(&maxHealth), sizeOfInt);
            lvl.read((char*)(&maxMoves), sizeOfInt);
            lvl.read((char*)(&team), sizeOfInt);
            Program* p = new Program((PROGRAM)type, team, {xHead, yHead});
            p->setMaxHealth(maxHealth);
            p->setMaxMoves(maxMoves);

            if (team == 0)
                //playerHumanList->getFirst()->addProgram(p);
                playerHumanList->getFirst()->addProgram(p);
            else if (team == 1)
                playerCompList->getFirst()->addProgram(p);
            else
                printf("err: a program was loaded with an invalid team\n");

            progs.addLast(p);
        }

        // load the grid from the file
        if (debug >= DEBUG_NORMAL) printf("loading tiles, items, and program pointers...\n");
        for (int x = left; x < right; x++)
        {
            for (int y = top; y < bottom; y++)
            {
                // tiles
                TILE typ;
                lvl.read((char*)(&typ), sizeOfInt);
                setTileAt({x, y}, typ);

                // items
                ITEM itm;
                lvl.read((char*)(&itm), sizeOfInt);
                gridItems[x][y] = itm;

                // programs
                int indx;
                lvl.read((char*)(&indx), sizeOfInt);
                gridPrograms[x][y] = progs.getObjectAt(indx);
                if (gridPrograms[x][y] != NULL) gridPrograms[x][y]->addTail({x, y});
            }
        }

        // close the file
        lvl.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void Game::removeReferencesToProgram(Program* p)
{
    if (p == NULL)
        return;

    for (int i = 0; i < 200; i++)
        for (int j = 0; j < 200; j++)
            if (gridPrograms[i][j] == p)
                gridPrograms[i][j] = NULL;
}

int Game::getLeftBound()
{
    return gridLeftBound;
}

int Game::getRightBound()
{
    return gridRightBound;
}

int Game::getTopBound()
{
    return gridTopBound;
}

int Game::getBottomBound()
{
    return gridBottomBound;
}

void Game::setBackground(BACKGROUND b)
{
    bkg_ = b;
}

BACKGROUND Game::getBackground()
{
    return bkg_;
}

void Game::setProgramAt(Coord pos, Program* p)
{
    // sanity check
    if (isOOB(pos))
        return;

    // place the program
    gridPrograms[pos.x][pos.y] = p;
}

void Game::setItemAt(Coord pos, ITEM i)
{
    // don't place an item if there's no tile here
    if (!isTiled(pos))
        return;

    // place the item here
    gridItems[pos.x][pos.y] = i;
}

TILE Game::getTileAt(Coord pos)
{
    if (pos.x >= 200 || pos.x < 0 || pos.y >= 200 || pos.y < 0)
        return TILE_NONE;
    return gridTiles[pos.x][pos.y];
}

Program* Game::getProgramAt(Coord pos)
{
    return gridPrograms[pos.x][pos.y];
}

ITEM Game::getItemAt(Coord pos)
{
    return gridItems[pos.x][pos.y];
}

bool Game::isOOB(Coord pos)
{
    if (pos.x < 0 || pos.x >= 200 || pos.y < 0 || pos.y >= 200)
        return true;
    else
        return false;
}

bool Game::isTiled(Coord pos)
{
    if (isOOB(pos) || gridTiles[pos.x][pos.y] == TILE_NONE)
        return false;
    else
        return true;
}

LinkedList<Player*>* Game::getHumanPlayers()
{
    return playerHumanList;
}

LinkedList<Player*>* Game::getAIPlayers()
{
    return playerCompList;
}

GAMESTATUS Game::getStatus()
{
	return status_;
}

void Game::setStatus(GAMESTATUS gs)
{
	switch (gs)
	{
	case GAMESTATUS_NO_GAME:
		break;
	case GAMESTATUS_PLACING_PROGRAMS:
		break;
	case GAMESTATUS_PLAYING:
		for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
			if (getTileAt({ x, y }) == TILE_SPAWN || getTileAt({ x, y }) == TILE_SPAWN2)
			{
				setTileAt({ x, y }, TILE_PLAIN);
			}
		break;
	case GAMESTATUS_WON:
		break;
	case GAMESTATUS_LOST:
		break;
	}

	status_ = gs;
}

Player* Game::getPlayerByID(int playerID)
{
	Iterator<Player*> itHuman = playerHumanList->getIterator();
	while (itHuman.hasNext())
	{
		Player* curr = itHuman.next();
		if (curr->getPlayerID() == playerID)
			return curr;
	}

	Iterator<Player*> itComp = playerCompList->getIterator();
	while (itComp.hasNext())
	{
		Player* curr = itComp.next();
		if (curr->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
}

void Game::moveProgramTo(Program* p, Coord c)
{
	p->moveTo(c);
	setProgramAt(c, p);
}

Player* Game::getCurrTurnPlayer()
{
	return currTurnPlayer;
}

void Game::setCurrTurnPlayer(Player* p)
{
	currTurnPlayer = p;
}