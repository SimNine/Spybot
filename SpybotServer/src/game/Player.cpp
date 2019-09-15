#include "Standard.h"
#include "Player.h"

#include "Game.h"
#include "Program.h"
#include "Message.h"
#include "Server.h"
#include "Main.h"
#include "MiscUtil.h"
#include "ProgramAction.h"

Player::Player(Game* g, int t)
{
    game = g;
    team = t;
    doneTurn = false;
	selectedTile = { -1, -1 };
	selectedProgram = NULL;
    progsOwned = new LinkedList<Program*>();
	playerID_ = -1;
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

	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_PROGRAM;
	m.clientID = 0;
	m.playerID = playerID_;
	if (p != NULL)
		m.programID = p->getProgramID();
	else
		m.programID = -1;
	server->sendMessageToAllClients(m);
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

		// move the program
        game->setProgramAt(pos, selectedProgram);
        selectedProgram->moveTo(pos);
        selectedTile = selectedProgram->getHead();
        calculateProgramDist(selectedProgram);

		// move the program for the client
		Message msg;
		msg.type = MSGTYPE_MOVE;
		msg.clientID = 0;
		msg.playerID = playerID_;
		msg.programID = selectedProgram->getProgramID();
		msg.pos = pos;
		server->sendMessageToAllClients(msg);

		// send the move sound to the client
        msg.type = MSGTYPE_SOUND;
        msg.soundType = MSGSOUNDNAME_MOVE;
        msg.numRepeats = 0;
        server->sendMessageToAllClients(msg);

        // if there is a credit here, pick it up
        if (game->getItemAt(pos) == ITEM_CREDIT && team == 0)
        {
            game->setItemAt(pos, ITEM_NONE);

            msg.soundType = MSGSOUNDNAME_PICKUPCREDIT;
            server->sendMessageToAllClients(msg);
        }
    }
}

void Player::moveSelectedProgramBy(Coord disp)
{
    Coord n = selectedProgram->getCore() + disp;
    moveSelectedProgram(n);
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
    Coord f = p->getCore();
    while (ll.getLength() > 0)
    {
        Coord* currP = ll.poll();
        Coord curr = *currP;
        delete currP;

        int farDist = dist(f, h);
        int currDist = dist(curr, h);

        if (currDist > farDist)
            f = curr;
    }

    return f;
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

    progsOwned->addLast(p);
    p->setOwner(this);
}

void Player::setSelectedTile(Coord pos)
{
	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_TILE;
	m.clientID = 0;
	m.playerID = playerID_;

    if (game->isOOB(pos))
    {
        setSelectedProgram(NULL);
        selectedTile = {-1, -1};
		
		m.pos = pos;
		server->sendMessageToAllClients(m);
        return;
    }

    setSelectedProgram(game->getProgramAt(pos));

	if (selectedProgram == NULL)
	{
		selectedTile = pos;
		m.pos = pos;
		server->sendMessageToAllClients(m);
	}
	else
	{
		selectedTile = selectedProgram->getHead();
		m.pos = selectedProgram->getHead();
		server->sendMessageToAllClients(m);
	}
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

void Player::setDoneTurn(bool b)
{
	doneTurn = b;
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

	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_ACTION;
	m.clientID = 0;
	m.playerID = playerID_;
	m.programID = selectedProgram->getProgramID();
	m.actionID = selectedProgram->getActions()->getIndexOf(pa);
	server->sendMessageToAllClients(m);
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
	// check for validity of move
    if (selectedAction == NULL || game->isOOB(pos) || selectedProgram->getActionsLeft() <= 0)
        return;

	// get program on target tile
    Program* tgtProg = game->getProgramAt(pos);

    switch (selectedAction->type)
    {
    case ACTIONTYPE_DAMAGE:
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
	case ACTIONTYPE_SPEEDDOWN:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getMaxMoves() < selectedAction->power)
			tgtProg->setMaxMoves(0);
		else
			tgtProg->setMaxMoves(tgtProg->getMaxMoves() - selectedAction->power);
		break;
    case ACTIONTYPE_SPEEDUP:
        if (tgtProg == NULL)
            return;

        tgtProg->setMaxMoves(tgtProg->getMaxMoves() + selectedAction->power);
        break;
    case ACTIONTYPE_TILEDELETE:
        if (game->isTiled(pos) && game->getProgramAt(pos) == NULL)
            game->setTileAt(pos, TILE_NONE);
        break;
    case ACTIONTYPE_TILEPLACE:
        if (!game->isTiled(pos))
            game->setTileAt(pos, TILE_PLAIN);
        break;
	case ACTIONTYPE_MAXHEALTHDOWN:
		if (tgtProg == NULL)
			return;

		if (tgtProg->getMaxHealth() < selectedAction->power - 1)
			tgtProg->setMaxHealth(0);
		else
			tgtProg->setMaxHealth(tgtProg->getMaxHealth() - selectedAction->power);
		break;
	case ACTIONTYPE_MAXHEALTHUP:
		if (tgtProg == NULL)
			return;

		tgtProg->setMaxHealth(tgtProg->getMaxHealth() + selectedAction->power);
		break;
	case ACTIONTYPE_HEAL:
		printf("SERVER ERR: action type HEAL not implemented yet\n");
    default:
        break;
    }

	selectedProgram->setActionsLeft(selectedProgram->getActionsLeft() - 1);
	setSelectedAction(NULL);

	Message m;
	m.type = MSGTYPE_ACTION;
	m.clientID = 0;
	m.playerID = playerID_;
	m.pos = pos;
	server->sendMessageToAllClients(m);
}

int Player::getPlayerID()
{
	return playerID_;
}

void Player::setPlayerID(int playerID)
{
	playerID_ = playerID;
}

Program* Player::getProgramByID(int progID)
{
	Iterator<Program*> it = progsOwned->getIterator();
	while (it.hasNext())
	{
		Program* curr = it.next();
		if (curr->getProgramID() == progID)
			return curr;
	}

	return NULL;
}

Game* Player::getGame()
{
	return game;
}

void Player::setMind(AICore* ai)
{
	mind_ = ai;
}

AICore* Player::getMind()
{
	return mind_;
}