#include "Player.h"
#include "Game.h"
#include "Global.h"
#include "MiscUtil.h"

Player::Player(Game* g, int t)
{
    game = g;
    team = t;
    doneTurn = false;
    setSelectedTile({-1, -1});
    progsOwned = new LinkedList<Program*>();
}

Player::~Player()
{
    while (progsOwned->getLength() > 0)
    {
        Program* p = progsOwned->poll();
        delete p;
    }
    delete progsOwned;
}

void Player::setSelectedProgram(Program* p)
{
    if (p == NULL) calculateProgramDist(NULL);
    else if (p->getTeam() != team) calculateProgramDist(NULL);
    else calculateProgramDist(p);

    setSelectedAction(NULL);
    selectedProgram = p;
}

void Player::moveSelectedProgram(Coord pos)
{
    // check for validity
    if (!game->isTiled(pos)) return;

    // if the given space is adjacent to the selected program,
    // and the selected program has moves left
    if (selectedProgDist[pos.x][pos.y] == 1 && selectedProgram->getMoves() > 0)
    {
        // delete the tail of this program if it's at max health
        if (selectedProgram->getHealth() == selectedProgram->getMaxHealth() &&
            game->getProgramAt(pos) != selectedProgram)
        {
            Coord temp = selectedProgram->getTail();
            game->setProgramAt(temp, NULL);
        }

        game->setProgramAt(pos, selectedProgram);
        selectedProgram->moveTo(pos);
        selectedTile = selectedProgram->getHead();
        calculateProgramDist(selectedProgram);
        Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
    }
}

void Player::moveSelectedProgramBy(Coord disp)
{
    // establish temp variables
    Coord n = selectedProgram->getCore() + disp;

    // check for validity
    if (!game->isTiled(n)) return;

    // if the given space is adjacent to the selected program,
    // and the selected program has moves left
    if (selectedProgDist[n.x][n.y] == 1 && selectedProgram->getMoves() > 0)
    {
        // delete the tail of this program if it's at max health
        if (selectedProgram->getHealth() == selectedProgram->getMaxHealth() &&
            game->getProgramAt(n) != selectedProgram)
        {
            Coord temp = selectedProgram->getTail();
            game->setProgramAt(temp, NULL);
        }

        game->setProgramAt(n, selectedProgram);
        selectedProgram->moveTo(n);
        selectedTile = selectedProgram->getHead();
        calculateProgramDist(selectedProgram);
        Mix_PlayChannel(-1, dataContainer->sound_move_player, 0);
    }
}

bool Player::canSelectedProgramMoveTo(Coord pos)
{
    // in case something breaks
    if (selectedProgram == NULL)
        return false;

    // in case this isn't your program
    if (selectedProgram->getOwner() != this)
        return false;

    //otherwise...
    if (selectedProgDist[pos.x][pos.y] == 1)
        return true;
    else
        return false;
}

bool Player::canSelectedProgramMoveBy(Coord disp)
{
    // in case something breaks
    if (selectedProgram == NULL)
        return false;

    //otherwise...
    if (selectedProgDist[selectedProgram->getCore().x + disp.x][selectedProgram->getCore().y + disp.y] >= 1)
        return true;
    else
        return false;
}

Program* Player::getSelectedProgram()
{
    return selectedProgram;
}

int Player::getSelectedProgramDist(Coord pos)
{
    return selectedProgDist[pos.x][pos.y];
}

int Player::getSelectedProgramDistAll(Coord pos)
{
    return selectedProgDistAll[pos.x][pos.y];
}

Coord Player::getFarthestTile(Program* p)
{
    LinkedList<Coord*> ll = LinkedList<Coord*>();
    for (int x = game->getLeftBound(); x < game->getRightBound(); x++)
        for (int y = game->getLeftBound(); y < game->getRightBound(); y++)
            if (game->getProgramAt({x, y}) == p)
                ll.addFirst(new Coord{x, y});

    Coord h = p->getCore();
    Coord far = p->getCore();
    while (ll.getLength() > 0)
    {
        Coord* currP = ll.poll();
        Coord curr = *currP;
        delete currP;

        int farDist = dist(far, h);
        int currDist = dist(curr, h);

        if (currDist > farDist)
            far = curr;
    }

    return far;
}

void Player::calculateProgramDist(Program* p)
{
    for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
    {
        selectedProgDist[x][y] = -1;
        selectedProgDistAll[x][y] = -1;
    }

    if (p == NULL)
        return;

    Coord h = p->getCore();
    selectedProgDist[h.x][h.y] = 0;
    selectedProgDistAll[h.x][h.y] = 0;

    LinkedList<Coord*> ll = LinkedList<Coord*>();
    ll.addFirst(new Coord(h));

    // for each pair in the list
    while (ll.getLength() > 0)
    {
        Coord* currP = ll.poll();
        Coord curr = *currP;
        delete currP;

        int dCurr = selectedProgDistAll[curr.x][curr.y];

        // check tile to the right
        if (curr.x + 1 < 200 &&
            game->getTileAt({curr.x+1, curr.y}) != TILE_NONE &&
            (game->getProgramAt({curr.x+1, curr.y}) == NULL || game->getProgramAt({curr.x+1, curr.y}) == p) &&
            selectedProgDistAll[curr.x+1][curr.y] == -1)
        {
            if (dCurr < p->getMoves())
                selectedProgDist[curr.x+1][curr.y] = dCurr + 1;
            selectedProgDistAll[curr.x+1][curr.y] = dCurr + 1;
            ll.addLast(new Coord{curr.x + 1, curr.y});
        }

        // check tile to the left
        if (curr.x - 1 >= 0 &&
            game->getTileAt({curr.x-1, curr.y}) != TILE_NONE &&
            (game->getProgramAt({curr.x-1, curr.y}) == NULL || game->getProgramAt({curr.x-1, curr.y}) == p) &&
            selectedProgDistAll[curr.x-1][curr.y] == -1)
        {
            if (dCurr < p->getMoves())
                selectedProgDist[curr.x-1][curr.y] = dCurr + 1;
            selectedProgDistAll[curr.x-1][curr.y] = dCurr + 1;
            ll.addLast(new Coord{curr.x - 1, curr.y});
        }

        // check tile below
        if (curr.y + 1 < 200 &&
            game->getTileAt({curr.x, curr.y+1}) != TILE_NONE &&
            (game->getProgramAt({curr.x, curr.y+1}) == NULL || game->getProgramAt({curr.x, curr.y+1}) == p) &&
            selectedProgDistAll[curr.x][curr.y+1] == -1)
        {
            if (dCurr < p->getMoves())
                selectedProgDist[curr.x][curr.y+1] = dCurr + 1;
            selectedProgDistAll[curr.x][curr.y+1] = dCurr + 1;
            ll.addLast(new Coord{curr.x, curr.y + 1});
        }

        // check tile above
        if (curr.y - 1 >= 0 &&
            game->getTileAt({curr.x, curr.y-1}) != TILE_NONE &&
            (game->getProgramAt({curr.x, curr.y-1}) == NULL || game->getProgramAt({curr.x, curr.y-1}) == p) &&
            selectedProgDistAll[curr.x][curr.y-1] == -1)
        {
            if (dCurr < p->getMoves())
                selectedProgDist[curr.x][curr.y-1] = dCurr + 1;
            selectedProgDistAll[curr.x][curr.y-1] = dCurr + 1;
            ll.addLast(new Coord{curr.x, curr.y-1});
        }
    }
}

void Player::addProgram(Program* p)
{
    if (progsOwned->contains(p))
        return;

    progsOwned->addFirst(p);
    p->setOwner(this);
}

void Player::setSelectedTile(Coord pos)
{
    if (game->isOOB(pos))
    {
        setSelectedProgram(NULL);
        selectedTile = {-1, -1};
        return;
    }

    setSelectedProgram(game->getProgramAt(pos));

    if (selectedProgram == NULL)
        selectedTile = pos;
    else
        selectedTile = selectedProgram->getHead();
}

Coord Player::getSelectedTile()
{
    return selectedTile;
}

void Player::endTurn()
{
    doneTurn = false;

    Iterator<Program*> it = progsOwned->getIterator();
    while (it.hasNext())
        it.next()->endTurn();

    setSelectedTile({-1, -1});
}

bool Player::getDoneTurn()
{
    return doneTurn;
}

LinkedList<Program*>* Player::getProgList()
{
    return progsOwned;
}

int Player::getTeam()
{
    return team;
}

void Player::setSelectedAction(ProgramAction* pa)
{
    for (int x = 0; x < 200; x++)
        for (int y = 0; y < 200; y++)
            selectedActionDist[x][y] = -1;

    if (selectedProgram == NULL || pa == NULL)
    {
        selectedAction = NULL;
        return;
    }

    Coord center = selectedProgram->getCore();
    for (int x = -pa->range; x <= pa->range; x++)
    {
        int rngLeft = pa->range - abs(x);
        for (int y = -rngLeft; y <= rngLeft; y++)
            if (game->isTiled(center + Coord{x, y}))
                selectedActionDist[center.x + x][center.y + y] = dist(center, center + Coord{x, y});
    }
    selectedActionDist[center.x][center.y] = 0;

    selectedAction = pa;
}

ProgramAction* Player::getSelectedAction()
{
    return selectedAction;
}

int Player::getSelectedActionDist(Coord pos)
{
    return selectedActionDist[pos.x][pos.y];
}

void Player::useSelectedActionAt(Coord pos)
{
    if (selectedAction == NULL || game->isOOB(pos))
        return;

    Program* tgtProg = game->getProgramAt(pos);

    switch (selectedAction->type)
    {
    case MOVETYPE_DAMAGE:
        if (tgtProg == NULL)
            return;

        if (tgtProg->getTeam() != team)
        {
            for (int i = 0; i < selectedAction->power; i++)
            {
                Coord* curr = tgtProg->popTail();
                if (curr == NULL)
                    break;
                else
                    game->setProgramAt(*curr, NULL);
            }
        }
        if (tgtProg->getHealth() <= 0)
        {
            Player* owner = tgtProg->getOwner();
            owner->getProgList()->remove(tgtProg);
            delete tgtProg;
        }
        break;
    case MOVETYPE_SPEEDUP:
        if (tgtProg == NULL)
            return;

        tgtProg->setMaxMoves(tgtProg->getMaxMoves() + selectedAction->power);
        break;
    case MOVETYPE_TILEDELETE:
        if (game->isTiled(pos) && game->getProgramAt(pos) == NULL)
            game->setTileAt(pos, TILE_NONE);
        break;
    case MOVETYPE_TILEPLACE:
        if (!game->isTiled(pos))
            game->setTileAt(pos, TILE_PLAIN);
        break;
    default:
        break;
    }
}
