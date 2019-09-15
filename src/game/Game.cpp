#include <stdio.h>
#include <fstream>

#include "Game.h"
#include "Global.h"
#include "Entry.h"

Game::Game()
{
    initBoard();
}

Game::Game(std::string lvlStr)
{
    initBoard();
    loadLevel(lvlStr);
}

Game::~Game()
{
    for (int x = 0; x < 200; x++)
    {
        for (int y = 0; y < 200; y++)
        {
            if (gridPrograms[x][y] != NULL)
            {
                Program* temp = gridPrograms[x][y];
                deleteProgram(temp);
            }
        }
    }
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
            gridProgramDist[x][y] = -1;
        }
    }

    gridLeftBound = 100;
    gridRightBound = 100;
    gridTopBound = 100;
    gridBottomBound = 100;

    selectedProgram = NULL;
}

void Game::setTileAt(int x, int y, TILE t)
{
    // check for OOB
    if (isOOB(x, y)) return;

    // set the tile
    gridTiles[x][y] = t;

    // try increasing bounds
    if (t != TILE_NONE)
    {
        if (x < gridLeftBound)
        {
            gridLeftBound = x;
        }
        else if (x + 1 > gridRightBound)
        {
            gridRightBound = x + 1;
        }

        if (y < gridTopBound)
        {
            gridTopBound = y;
        }
        else if (y + 1 > gridBottomBound)
        {
            gridBottomBound = y + 1;
        }
    }
    else // try decreasing bounds
    {
        if (x == gridLeftBound)
        {
            bool b = true;
            while (b && gridLeftBound < 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[gridLeftBound][i] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridLeftBound++;
                }
            }
        }
        else if (x + 1 == gridRightBound)
        {
            bool b = true;
            while (b && gridRightBound > 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[gridRightBound - 1][i] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridRightBound--;
                }
            }
        }

        if (y == gridTopBound)
        {
            bool b = true;
            while (b && gridTopBound < 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[i][gridTopBound] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridTopBound++;
                }
            }
        }
        else if (y + 1 == gridBottomBound)
        {
            bool b = true;
            while (b && gridBottomBound > 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[i][gridBottomBound - 1] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridBottomBound--;
                }
            }
        }
    }
}

bool Game::isDrawValid(int x, int y, int height, int width)
{
    if (x < 0 || x + width > 200 ||
            y < 0 || y + height > 200)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Game::drawRectInBoard(TILE tileType, int x, int y, int width, int height)
{
    if (!isDrawValid(x, y, width, height))
    {
        return;
    }

    for (int xx = x; xx < x + width; xx++)
    {
        setTileAt(xx, y, tileType);
        setTileAt(xx, y + height - 1, tileType);
    }
    for (int yy = y; yy < y + height; yy++)
    {
        setTileAt(x, yy, tileType);
        setTileAt(x + width - 1, yy, tileType);
    }
}

void Game::fillRectInBoard(TILE tileType, int x, int y, int width, int height)
{
    if (!isDrawValid(x, y, width, height))
    {
        return;
    }

    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            setTileAt(i, j, tileType);
        }
    }
}

void Game::drawOvalInBoard(TILE tileType, int x, int y, int width, int height)
{

}

void Game::fillOvalInBoard(TILE tileType, int x, int y, int width, int height)
{
    int xCenter = x + width/2;
    int yCenter = y + height/2;
    for (int i = x; i < x + width - 1; i++)
    {
        for (int j = y; j < y + height - 1; j++)
        {
            if (((i - xCenter)*(i - xCenter))/((width/2)*(width/2)) &&
                    ((j - yCenter)*(j - yCenter))/((height/2)*(height/2)))
            {
                setTileAt(i, j, tileType);
            }
        }
    }
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
        lvl.write((char*) &bkg, sizeOfInt);

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

            int xHead = currProg->getCoreX();
            lvl.write((char*)(&xHead), sizeOfInt);
            int yHead = currProg->getCoreY();
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
        lvl.read((char*) &bkg, sizeOfInt);
        setBackground(bkg);

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
            Program* p = new Program((PROGRAM)type, team, xHead, yHead);
            //p->setHealth(health);
            p->setMaxHealth(maxHealth);
            p->setMaxMoves(maxMoves);
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
                setTileAt(x, y, typ);

                // items
                ITEM itm;
                lvl.read((char*)(&itm), sizeOfInt);
                gridItems[x][y] = itm;

                // programs
                int indx;
                lvl.read((char*)(&indx), sizeOfInt);
                gridPrograms[x][y] = progs.getObjectAt(indx);
                if (gridPrograms[x][y] != NULL) gridPrograms[x][y]->addTail(x, y);
            }
        }

        // close the file
        lvl.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void Game::deleteProgram(Program* p)
{
    if (p == NULL) return;

    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            if (gridPrograms[i][j] == p)
            {
                gridPrograms[i][j] = NULL;
            }
        }
    }
    delete p;
}

void Game::calculateProgramDist(Program* p)
{
    for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++) gridProgramDist[x][y] = -1;

    if (p == NULL) return;

    int xH = p->getCoreX();
    int yH = p->getCoreY();

    gridProgramDist[xH][yH] = 0;
    LinkedList<Pair<int>*> ll = LinkedList<Pair<int>*>();
    ll.addFirst(new Pair<int>(xH, yH));

    // for each pair in the list
    while (ll.getLength() > 0)
    {
        Pair<int>* currPair = ll.poll();

        int xCurr = currPair->a;
        int yCurr = currPair->b;
        int dCurr = gridProgramDist[xCurr][yCurr];

        // if this pair is the edge of the program's range
        if (dCurr < p->getMoves())
        {
            // check tile to the right
            if (xCurr + 1 < 200 &&
                    gridTiles[xCurr+1][yCurr] != TILE_NONE &&
                    (gridPrograms[xCurr+1][yCurr] == NULL || gridPrograms[xCurr+1][yCurr] == p) &&
                    gridProgramDist[xCurr+1][yCurr] == -1)
            {
                gridProgramDist[xCurr+1][yCurr] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr + 1, yCurr));
            }

            // check tile to the left
            if (xCurr - 1 >= 0 &&
                    gridTiles[xCurr-1][yCurr] != TILE_NONE &&
                    (gridPrograms[xCurr-1][yCurr] == NULL || gridPrograms[xCurr-1][yCurr] == p) &&
                    gridProgramDist[xCurr-1][yCurr] == -1)
            {
                gridProgramDist[xCurr-1][yCurr] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr - 1, yCurr));
            }

            // check tile below
            if (yCurr + 1 < 200 &&
                    gridTiles[xCurr][yCurr+1] != TILE_NONE &&
                    (gridPrograms[xCurr][yCurr+1] == NULL || gridPrograms[xCurr][yCurr+1] == p) &&
                    gridProgramDist[xCurr][yCurr+1] == -1)
            {
                gridProgramDist[xCurr][yCurr+1] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr, yCurr + 1));
            }

            // check tile above
            if (yCurr - 1 >= 0 &&
                    gridTiles[xCurr][yCurr-1] != TILE_NONE &&
                    (gridPrograms[xCurr][yCurr-1] == NULL || gridPrograms[xCurr][yCurr-1] == p) &&
                    gridProgramDist[xCurr][yCurr-1] == -1)
            {
                gridProgramDist[xCurr][yCurr-1] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr, yCurr - 1));
            }
        }
        delete currPair;
    }
}

Pair<int>* Game::getFarthestTile(Program* p)
{
    LinkedList<Pair<int>*> ll = LinkedList<Pair<int>*>();
    for (int x = gridLeftBound; x < gridRightBound; x++)
    {
        for (int y = gridTopBound; y < gridBottomBound; y++)
        {
            if (gridPrograms[x][y] == p) ll.addFirst(new Pair<int>(x, y));
        }
    }

    int xH = p->getCoreX();
    int yH = p->getCoreY();
    Pair<int>* far = new Pair<int>(p->getCoreX(), p->getCoreY());
    while (ll.getLength() > 0)
    {
        Pair<int>* curr = ll.poll();
        int farDist = (far->a-xH)*(far->a-xH) + (far->b-yH)*(far->b-yH);
        int currDist = (curr->a-xH)*(curr->a-xH) + (curr->b-yH)*(curr->b-yH);

        if (currDist > farDist)
        {
            delete far;
            far = curr;
        }
        else
        {
            delete curr;
        }
    }

    return far;
}

Pair<int>* Game::getFarthestContiguousTile(Program* p)
{
    for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++) gridCheckedTile[x][y] = false;

    LinkedList<Entry<Pair<int>*, int>*> ll = LinkedList<Entry<Pair<int>*, int>*>();
    Entry<Pair<int>*, int>* maxDist = new Entry<Pair<int>*, int>(new Pair<int>(p->getCoreX(), p->getCoreY()), 0);
    ll.addFirst(maxDist);

    while (ll.getLength() > 0)
    {
        Entry<Pair<int>*, int>* curr = ll.poll();
        gridCheckedTile[curr->a->a][curr->a->b] = true;

        if (getTileAt(curr->a->a+1, curr->a->b) != TILE_NONE &&
            gridPrograms[curr->a->a+1][curr->a->b] == p &&
            gridCheckedTile[curr->a->a+1][curr->a->b] == false)
        {
            ll.addFirst(new Entry<Pair<int>*, int>(new Pair<int>(curr->a->a+1, curr->a->b), curr->b+1));
        }
        if (getTileAt(curr->a->a-1, curr->a->b) != TILE_NONE &&
            gridPrograms[curr->a->a-1][curr->a->b] == p &&
            gridCheckedTile[curr->a->a-1][curr->a->b] == false)
        {
            ll.addFirst(new Entry<Pair<int>*, int>(new Pair<int>(curr->a->a-1, curr->a->b), curr->b+1));
        }
        if (getTileAt(curr->a->a, curr->a->b+1) != TILE_NONE &&
            gridPrograms[curr->a->a][curr->a->b+1] == p &&
            gridCheckedTile[curr->a->a][curr->a->b+1] == false)
        {
            ll.addFirst(new Entry<Pair<int>*, int>(new Pair<int>(curr->a->a, curr->a->b+1), curr->b+1));
        }
        if (getTileAt(curr->a->a, curr->a->b-1) != TILE_NONE &&
            gridPrograms[curr->a->a][curr->a->b-1] == p &&
            gridCheckedTile[curr->a->a][curr->a->b-1] == false)
        {
            ll.addFirst(new Entry<Pair<int>*, int>(new Pair<int>(curr->a->a, curr->a->b-1), curr->b+1));
        }

        if (curr->b >= maxDist->b)
        {
            maxDist = curr;
        }
        else
        {
            delete curr->a;
            delete curr;
        }
    }

    Pair<int>* ret = maxDist->a;
    delete maxDist;
    return ret;
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

void Game::moveSelectedProgram(int x, int y)
{
    // check for validity
    if (!isTiled(x, y)) return;

    // if the given space is adjacent to the selected program,
    // and the selected program has moves left
    if (gridProgramDist[x][y] == 1 && selectedProgram->getMoves() > 0)
    {
        if (selectedProgram->getHealth() == selectedProgram->getMaxHealth() &&
            gridPrograms[x][y] != selectedProgram)
        {
            Pair<int>* temp = selectedProgram->getTail();
            gridPrograms[temp->a][temp->b] = NULL;
        }

        gridPrograms[x][y] = selectedProgram;
        selectedProgram->moveTo(x, y);
        Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
    }
}

Program* Game::getSelectedProgram()
{
    return selectedProgram;
}

void Game::setBackground(BACKGROUND b)
{
    bkg = b;
}

BACKGROUND Game::getBackground()
{
    return bkg;
}

void Game::setProgramAt(int x, int y, Program* p)
{
    // don't place a program here if there's no tile here
    if (!isTiled(x, y)) return;

    // if the pointer is null, remove the program here instead
    if (p == NULL)
    {
        deleteProgram(gridPrograms[x][y]);
        return;
    }

    // place the program here
    gridPrograms[x][y] = p;
    p->moveTo(x, y);
}

void Game::setItemAt(int x, int y, ITEM i)
{
    // don't place an item if there's no tile here
    if (!isTiled(x, y)) return;

    // place the item here
    gridItems[x][y] = i;
}

void Game::setSelectedProgram(int x, int y)
{
    if (isOOB(x, y))
    {
        calculateProgramDist(NULL);
        selectedProgram = NULL;
    }
    else
    {
        calculateProgramDist(gridPrograms[x][y]);
        selectedProgram = gridPrograms[x][y];
    }
}

void Game::endTurn()
{
    for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++) if (gridPrograms[x][y] != NULL) gridPrograms[x][y]->endTurn();
}

TILE Game::getTileAt(int x, int y)
{
    if (x >= 200 || x < 0 || y >= 200 || y < 0) return TILE_NONE;
    return gridTiles[x][y];
}

Program* Game::getProgramAt(int x, int y)
{
    return gridPrograms[x][y];
}

ITEM Game::getItemAt(int x, int y)
{
    return gridItems[x][y];
}

int Game::getSelectedProgramDist(int x, int y)
{
    return gridProgramDist[x][y];
}

bool Game::isOOB(int x, int y)
{
    if (x < 0 || x >= 200 || y < 0 || y >= 200) return true;
    else return false;
}

bool Game::isTiled(int x, int y)
{
    if (isOOB(x, y) || gridTiles[x][y] == TILE_NONE) return false;
    else return true;
}
