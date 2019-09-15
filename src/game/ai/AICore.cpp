#include "AICore.h"
#include "Player.h"
#include "MiscUtil.h"

AICore::AICore(Game* g, int t)
    : Player(g, t)
{
    // nothing
}

AICore::~AICore()
{
    //dtor
}

void AICore::act(int num)
{
    //printf("agent on team %i carrying out %i action steps...\n", team, num);
    if (num == -1)
    {
        bool b = true;
        while (b)
            b = actStep();
        doneTurn = true;
    }
    else
        for (int i = 0; i < num; i++)
            if (!actStep())
                doneTurn = true;
}


// gets the next program that still has moves or actions left in this turn
Program* AICore::getNextUsableProgram()
{
    Iterator<Program*> it = progsOwned->getIterator();
    Program* myProg = NULL;
    while (it.hasNext())
    {
        myProg = it.next();
        if (myProg->getMoves() > 0 || myProg->getActionsLeft() > 0)
            break;
        myProg = NULL;
    }
    return myProg;
}

// returns the program belonging to the given player whose core is
// closest to the core of this player's selected program.
// this algorithm takes no regard in terms of occupied or missing tiles.
// if passed NULL as targeted enemy, will check all enemy players
Program* AICore::getNearestEnemyProgramAbsoluteDist(Player* enemy)
{
    LinkedList<Program*>* allEnemyProgs = getAllEnemyPrograms();
    LinkedList<Program*>* enemyProgList = NULL;
    if (enemy == NULL)
        enemyProgList = allEnemyProgs;
    else
        enemyProgList = enemy->getProgList();

    Coord nearest = {INT_MAX, INT_MAX};
    Iterator<Program*> enemyProgs = enemyProgList->getIterator();
    while (enemyProgs.hasNext())
    {
        Program* enemyProg = enemyProgs.next();
        int newDist = dist(enemyProg->getCore(), selectedProgram->getCore());
        int oldDist = dist(nearest, selectedProgram->getCore());
        if (newDist < oldDist) nearest = enemyProg->getHead();
    }

    delete allEnemyProgs;
    return game->getProgramAt(nearest);
}

// returns the program belonging to the given player whose core is
// closest to the core of this player's selected program.
// this algorithm accounts for missing, but not occupied tiles
// if passed NULL as targeted enemy, will check all enemy players
Program* AICore::getNearestEnemyProgramAStarDist(Player* enemy)
{
    LinkedList<Program*>* allEnemyProgs = getAllEnemyPrograms();
    LinkedList<Program*>* enemyProgList = NULL;
    if (enemy == NULL)
        enemyProgList = allEnemyProgs;
    else
        enemyProgList = enemy->getProgList();

    Coord nearest = {INT_MAX, INT_MAX};
    Iterator<Program*> enemyProgs = enemyProgList->getIterator();
    while (enemyProgs.hasNext())
    {
        Program* enemyProg = enemyProgs.next();
        int newDist = getSelectedProgramDistAll({enemyProg->getCore().x, enemyProg->getCore().y});
        int oldDist = getSelectedProgramDistAll({nearest.x, nearest.y});
        if (newDist == -1)
            continue;
        else if (newDist < oldDist)
            nearest = enemyProg->getHead();
    }

    delete allEnemyProgs;
    return game->getProgramAt(nearest);
}

// returns a linked list containing a pointer to every enemy (i.e., not
// on this AI's team) on the grid
LinkedList<Program*>* AICore::getAllEnemyPrograms()
{
    LinkedList<Program*>* progs = new LinkedList<Program*>();
    Iterator<AICore*> itAI = game->getAIPlayers()->getIterator();
    while (itAI.hasNext())
    {
        Player* curr = itAI.next();
        if (curr->getTeam() != this->team)
            progs->addAllFirst(curr->getProgList());
    }
    if (game->getHumanPlayer()->getTeam() != this->team)
        progs->addAllFirst(game->getHumanPlayer()->getProgList());

    //printf("found %i enemy progs\n", progs.getLength());
    return progs;
}

Coord* AICore::idealStepTowardsProgram(Program* p)
{

}

Coord* AICore::naiveStepTowardsProgram(Program* p)
{
    // create temp variables
    Coord enemyCore = p->getCore();
    Coord myCore = selectedProgram->getCore();

    // gather the directions it would be optimal for this program to move
    LinkedList<Coord*> idealMoves = LinkedList<Coord*>();
    if (enemyCore.x < myCore.x && canSelectedProgramMoveBy({-1, 0}))
        idealMoves.addFirst(new Coord{-1, 0});
    if (enemyCore.x > myCore.x && canSelectedProgramMoveBy({1, 0}))
        idealMoves.addFirst(new Coord{1, 0});
    if (enemyCore.y < myCore.y && canSelectedProgramMoveBy({0, -1}))
        idealMoves.addFirst(new Coord{0, -1});
    if (enemyCore.y > myCore.y && canSelectedProgramMoveBy({0, 1}))
        idealMoves.addFirst(new Coord{0, 1});

    Coord* ret = NULL;

    // if there are no ideal moves
    if (idealMoves.getLength() == 0)
    {
        // gather marginally acceptable moves
        LinkedList<Coord*> nonidealMoves = LinkedList<Coord*>();
        if (enemyCore.x < myCore.x || enemyCore.x > myCore.x)
        {
            if (canSelectedProgramMoveBy({0, -1}))
                nonidealMoves.addFirst(new Coord{0, -1});
            if (canSelectedProgramMoveBy({0, 1}))
                nonidealMoves.addFirst(new Coord{0, 1});
        }
        if (enemyCore.y < myCore.y || enemyCore.y > myCore.y)
        {
            if (canSelectedProgramMoveBy({-1, 0}))
                nonidealMoves.addFirst(new Coord{-1, 0});
            if (canSelectedProgramMoveBy({1, 0}))
                nonidealMoves.addFirst(new Coord{1, 0});
        }

        // if there are no marginally accaptable moves, just idle
        if (nonidealMoves.getLength() == 0)
            ret = NULL;
        else // otherwise, pick one of the marginally acceptable moves
        {
            Coord* temp = nonidealMoves.getObjectAt(rand() % nonidealMoves.getLength());
            ret = new Coord{temp->x, temp->y};
            while (idealMoves.getLength() > 0) delete idealMoves.poll();
        }
    }
    else // otherwise, pick one of the ideal moves
    {
        Coord* temp = idealMoves.getObjectAt(rand() % idealMoves.getLength());
        ret = new Coord{temp->x, temp->y};
        while (idealMoves.getLength() > 0) delete idealMoves.poll();
    }

    return ret;
}
