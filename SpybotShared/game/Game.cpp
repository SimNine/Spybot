#include "Standard.h"
#include "Game.h"

#include "Global.h"
#include "AIBasic.h"
#include "Program.h"
#include "Message.h"
#include "Server.h"
#include "Player.h"
#include "MiscUtil.h"
#include "Team.h"
#include "User.h"
#include "Pipe.h"
#include "SpawnGroup.h"

Game::Game(std::string lvlStr) {
	teamList_ = new LinkedList<Team*>();
	spawnGroupList_ = new LinkedList<SpawnGroup*>();
	initBoard();
	loadLevel(lvlStr);
	//status_ = GAMESTATUS_NONE;
}

Game::~Game() {
	while (teamList_->getLength() > 0)
		delete teamList_->poll();
	delete teamList_;

	while (spawnGroupList_->getLength() > 0)
		delete spawnGroupList_->poll();
	delete spawnGroupList_;
}

void Game::initBoard() {
	for (int x = 0; x < 200; x++) {
		for (int y = 0; y < 200; y++) {
			gridTiles_[x][y] = TILE_NONE;
			gridItems_[x][y] = ITEM_NONE;
			gridPrograms_[x][y] = NULL;
		}
	}

	gridLeftBound_ = 100;
	gridRightBound_ = 100;
	gridTopBound_ = 100;
	gridBottomBound_ = 100;
}

SpawnGroup* Game::getUnassignedSpawnGroup() {
	if (spawnGroupList_->getLength() == 0)
		return NULL;

	Iterator<SpawnGroup*> it = spawnGroupList_->getIterator();
	while (it.hasNext()) {
		SpawnGroup* currGroup = it.next();
		if (currGroup->getPlayerID() == -1)
			return currGroup;
	}

	return NULL;
}

SpawnGroup* Game::getSpawnGroupByID(int groupID) {
	Iterator<SpawnGroup*> it = spawnGroupList_->getIterator();
	while (it.hasNext()) {
		SpawnGroup* curr = it.next();
		if (curr->getGroupID() == groupID) {
			return curr;
		}
	}

	return NULL;
}

SpawnGroup* Game::getSpawnGroupAt(Coord pos) {
	Iterator<SpawnGroup*> it = spawnGroupList_->getIterator();
	while (it.hasNext()) {
		SpawnGroup* curr = it.next();
		if (curr->containsTile(pos)) {
			return curr;
		}
	}

	return NULL;
}

LinkedList<SpawnGroup*>* Game::getSpawnGroups() {
	return spawnGroupList_;
}

void Game::setTileAt(Coord pos, TILE t) {
	// check for OOB
	if (isOOB(pos))
		return;

	// check if this tile was being changed from a spawntile to something else
	if ((gridTiles_[pos.x][pos.y] == TILE_SPAWN || gridTiles_[pos.x][pos.y] == TILE_SPAWN2) &&
		t != TILE_SPAWN && 
		t != TILE_SPAWN2) {
		SpawnGroup* group = getSpawnGroupAt(pos);
		if (group != NULL) {
			group->removeTile(pos);
		}
	}

	// set the tile
	gridTiles_[pos.x][pos.y] = t;

	// try increasing bounds
	if (t != TILE_NONE) {
		if (pos.x < gridLeftBound_)
			gridLeftBound_ = pos.x;
		else if (pos.x + 1 > gridRightBound_)
			gridRightBound_ = pos.x + 1;

		if (pos.y < gridTopBound_)
			gridTopBound_ = pos.y;
		else if (pos.y + 1 > gridBottomBound_)
			gridBottomBound_ = pos.y + 1;
	} else { // try decreasing bounds
		if (pos.x == gridLeftBound_) {
			bool b = true;
			while (b && gridLeftBound_ < 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[gridLeftBound_][i] != TILE_NONE)
						b = false;
				if (b)
					gridLeftBound_++;
			}
		} else if (pos.x + 1 == gridRightBound_) {
			bool b = true;
			while (b && gridRightBound_ > 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[gridRightBound_ - 1][i] != TILE_NONE)
						b = false;
				if (b)
					gridRightBound_--;
			}
		}

		if (pos.y == gridTopBound_) {
			bool b = true;
			while (b && gridTopBound_ < 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[i][gridTopBound_] != TILE_NONE)
						b = false;
				if (b)
					gridTopBound_++;
			}
		} else if (pos.y + 1 == gridBottomBound_) {
			bool b = true;
			while (b && gridBottomBound_ > 100) {
				for (int i = 0; i < 200; i++)
					if (gridTiles_[i][gridBottomBound_ - 1] != TILE_NONE)
						b = false;
				if (b)
					gridBottomBound_--;
			}
		}
	}

	// send a message indicating this tile was placed here to all clients
	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_TILE;
	m.tileType = t;
	m.pos = pos;
	_server->sendMessageToAllClients(m);
}

bool Game::isDrawValid(Coord pos, Coord dims) {
	if (pos.x < 0 || pos.x + dims.x > 200 ||
		pos.y < 0 || pos.y + dims.y > 200)
		return false;
	else
		return true;
}

void Game::drawRectInBoard(TILE tileType, Coord pos, Coord dims) {
	if (!isDrawValid(pos, dims))
		return;

	for (int xx = pos.x; xx < pos.x + dims.x; xx++) {
		setTileAt({ xx, pos.y }, tileType);
		setTileAt({ xx, pos.y + dims.y - 1 }, tileType);
	}
	for (int yy = pos.y; yy < pos.y + dims.y; yy++) {
		setTileAt({ pos.x, yy }, tileType);
		setTileAt({ pos.x + dims.x - 1, yy }, tileType);
	}
}

void Game::fillRectInBoard(TILE tileType, Coord pos, Coord dims) {
	if (!isDrawValid(pos, dims)) return;

	for (int i = pos.x; i < pos.x + dims.x; i++)
		for (int j = pos.y; j < pos.y + dims.y; j++)
			setTileAt({ i, j }, tileType);
}

void Game::drawOvalInBoard(TILE tileType, Coord pos, Coord dims) {

}

void Game::fillOvalInBoard(TILE tileType, Coord pos, Coord dims) {
	Coord center = { pos.x + dims.x / 2, pos.y + dims.y / 2 };
	for (int i = pos.x; i < pos.x + dims.x - 1; i++)
		for (int j = pos.y; j < pos.y + dims.y - 1; j++)
			if (((i - center.x)*(i - center.x)) / ((dims.x / 2)*(dims.x / 2)) &&
				((j - center.y)*(j - center.y)) / ((dims.y / 2)*(dims.y / 2)))
				setTileAt({ i, j }, tileType);
}

int Game::getLeftBound() {
	return gridLeftBound_;
}

int Game::getRightBound() {
	return gridRightBound_;
}

int Game::getTopBound() {
	return gridTopBound_;
}

int Game::getBottomBound() {
	return gridBottomBound_;
}

void Game::setBackground(BACKGROUND b) {
	bkg_ = b;

	// send message to clients indicating this change in background
	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_BKG;
	m.bkgType = b;
	_server->sendMessageToAllClients(m);
}

BACKGROUND Game::getBackground() {
	return bkg_;
}

void Game::setItemAt(Coord pos, ITEM i) {
	// don't place an item if there's no tile here
	if (!isTiled(pos))
		return;

	// place the item here
	gridItems_[pos.x][pos.y] = i;

	// send a message indicating this item was placed here to all clients
	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_ITEM;
	m.itemType = i;
	m.pos = pos;
	_server->sendMessageToAllClients(m);
}

TILE Game::getTileAt(Coord pos) {
	if (pos.x >= 200 || pos.x < 0 || pos.y >= 200 || pos.y < 0)
		return TILE_NONE;
	return gridTiles_[pos.x][pos.y];
}

void Game::setProgramAt(Coord pos, Program* p) {
	gridPrograms_[pos.x][pos.y] = p;
}

Program* Game::getProgramAt(Coord pos) {
	return gridPrograms_[pos.x][pos.y];
}

ITEM Game::getItemAt(Coord pos) {
	return gridItems_[pos.x][pos.y];
}

bool Game::isOOB(Coord pos) {
	if (pos.x < 0 || pos.x >= 200 || pos.y < 0 || pos.y >= 200)
		return true;
	else
		return false;
}

bool Game::isTiled(Coord pos) {
	if (isOOB(pos) || gridTiles_[pos.x][pos.y] == TILE_NONE)
		return false;
	else
		return true;
}

Player* Game::getPlayerByID(int playerID) {
	Iterator<Team*> itTeams = teamList_->getIterator();
	while (itTeams.hasNext()) {
		Team* currTeam = itTeams.next();
		Player* p = currTeam->getPlayerByID(playerID);
		if (p != NULL)
			return p;
	}

	return NULL;
}

GAMESTATUS Game::getStatus() {
	return status_;
}

void Game::setStatus(GAMESTATUS g) {
	switch (g) {
	case GAMESTATUS_NONE:
		break;
	case GAMESTATUS_PREGAME:
		break;
	case GAMESTATUS_PLAYING:
		for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
			if (getTileAt({ x, y }) == TILE_SPAWN || getTileAt({ x, y }) == TILE_SPAWN2) {
				// convert all spawntiles to plain tiles
				setTileAt({ x, y }, TILE_PLAIN);
			}
		{
			// set the turn to be the first player
			currTurnPlayer_ = teamList_->getFirst()->getAllPlayers()->getFirst();

			Message msg;
			msg.type = MSGTYPE_NEXTTURN;
			msg.clientID = 0;
			msg.playerID = currTurnPlayer_->getPlayerID();
			_server->sendMessageToAllClients(msg);
		}
		break;
	case GAMESTATUS_END:
		break;
	}

	status_ = g;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_GAMESTATUS;
	m.statusType = g;
	_server->sendMessageToAllClients(m);
}

Player* Game::getCurrTurnPlayer() {
	return currTurnPlayer_;
}

void Game::setCurrTurnPlayer(Player* p) {
	currTurnPlayer_ = p;

	// send a message saying it's this player's turn
	Message m;
	m.type = MSGTYPE_NEXTTURN;
	m.clientID = 0;
	m.playerID = p->getPlayerID();
	_server->sendMessageToAllClients(m);
}

LinkedList<Team*>* Game::getAllTeams() {
	return teamList_;
}

Team* Game::getTeamByID(int teamNum) {
	Iterator<Team*> it = teamList_->getIterator();
	while (it.hasNext()) {
		Team* curr = it.next();
		if (curr->getTeamID() == teamNum)
			return curr;
	}

	return NULL;
}

Team* Game::getDefaultTeamAI() {
	Iterator<Team*> it = teamList_->getIterator();
	while (it.hasNext()) {
		Team* curr = it.next();
		if (curr->isDefaultAI())
			return curr;
	}

	return NULL;
}

Team* Game::getDefaultTeamHuman() {
	Iterator<Team*> it = teamList_->getIterator();
	while (it.hasNext()) {
		Team* curr = it.next();
		if (curr->isDefaultHuman())
			return curr;
	}

	return NULL;
}

Player* Game::getFollowingPlayer(Player* currPlayer) {
	Team* currTeam = getTeamByID(currPlayer->getTeam());

	int teamIndex = teamList_->getIndexOf(currTeam);
	int playerIndex = currTeam->getAllPlayers()->getIndexOf(currPlayer);

	// if this is the last player on the current team
	if (currTeam->getAllPlayers()->getLast() == currPlayer) {
		// if this is the last team
		if (teamList_->getLast() == currTeam) {
			return teamList_->getFirst()->getAllPlayers()->getFirst();
		} else {
			return teamList_->getObjectAt(teamIndex + 1)->getAllPlayers()->getFirst();
		}
	} else { // if this is NOT the last player on the current team
		return currTeam->getAllPlayers()->getObjectAt(playerIndex + 1);
	}

	return NULL;
}

void Game::checkForWinCondition() {
	int currTeamWinning = -1;

	Iterator<Team*> itTeam = teamList_->getIterator();
	while (itTeam.hasNext()) { // check each team
		Team* currTeam = itTeam.next();
		Iterator<Player*> itPlayer = currTeam->getAllPlayers()->getIterator();
		while (itPlayer.hasNext()) { // check each player on this team to see if they are alive
			Player* currPlayer = itPlayer.next();
			if (currPlayer->getProgList()->getLength() > 0) { // if this player is alive
				if (currTeamWinning == -1) { // if there is not yet a winning team
					currTeamWinning = currTeam->getTeamID();
				} else { // if this team is alive, and is a different team than previously (no winning condition)
					return;
				}
			}
		}
	}

	// at this point, if currTeamWinning is -1, something went wrong
	// otherwise, currTeamWinning is the number of the winning team
	if (currTeamWinning != -1) {
		Message m;
		m.type = MSGTYPE_INFO;
		m.infoType = MSGINFOTYPE_GAMESTATUS;
		m.statusType = GAMESTATUS_END;
		m.teamID = currTeamWinning;
		_server->sendMessageToAllClients(m);
		log("SERVER: game detected winning condition for team " + to_string(currTeamWinning) + "\n");
	}

	// refund all players' programs
	Iterator<Pipe*> it = _server->getClientList()->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		User* currUser = _server->getUserByName(curr->getUser());
		if (currUser != NULL) {
			for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
				if (currUser->progsInPlay_[i] > 0) {
					currUser->progsOwned_[i] += currUser->progsInPlay_[i];
					currUser->progsInPlay_[i] = 0;

					Message m;
					m.type = MSGTYPE_PROGINVENTORY;
					m.progType = (PROGRAM)i;
					m.num = currUser->progsOwned_[i];
					_server->sendMessageToClient(m, curr->getClientID());
				}
			}
		}
		_server->saveUsers();
	}

	// send all players on the winning team a winning message
	if (_server->getSavePath() == "levels/classic" || _server->getSavePath() == "levels/nightfall") {
		Team* t = this->getTeamByID(currTeamWinning);
		Iterator<Player*> itPlayers = t->getAllPlayers()->getIterator();
		while (itPlayers.hasNext()) {
			Player* currPlayer = itPlayers.next();
			Iterator<Pipe*> itPipes = _server->getClientList()->getIterator();
			while (itPipes.hasNext()) {
				Pipe* currPipe = itPipes.next();
				if (currPlayer->getPlayerID() == currPipe->getPlayer()) {
					User* user = _server->getUserByName(currPipe->getUser());

					// update this user's progress
					if (_server->getSavePath() == "levels/classic") {
						user->campaignClassic_[atoi(_server->getCurrentLevel().c_str())] = true;
						_server->saveUsers();
					} else if (_server->getSavePath() == "levels/nightfall") {
						user->campaignNightfall_[atoi(_server->getCurrentLevel().c_str())] = true;
						_server->saveUsers();
					}

					// send message letting client know of level unlock
					Message m;
					m.type = MSGTYPE_LEVELUNLOCK;
					m.num = atoi(_server->getCurrentLevel().c_str());
					currPipe->sendData(m);
				}
			}
		}
	}
}

Team* Game::addTeam() {
	Team* t = new Team();
	teamList_->addFirst(t);

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_TEAM;
	m.teamID = t->getTeamID();
	_server->sendMessageToAllClients(m);

	return t;
}

Player* Game::addPlayer(int teamID) {
	Team* t = getTeamByID(teamID);
	if (t == NULL) {
		log("SERVER ERR: tried adding new player to team " + to_string(teamID) + ", which doesn't exist\n");
		return NULL;
	}

	Player* p = new Player(this, teamID);
	t->getAllPlayers()->addFirst(p);

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PLAYER;
	m.teamID = teamID;
	m.playerID = p->getPlayerID();
	_server->sendMessageToAllClients(m);

	return p;
}

Program* Game::addProgram(PROGRAM type, int playerID, int teamID) {
	Team* t = getTeamByID(teamID);
	if (t == NULL) {
		log("SERVER ERR: tried adding new program to player " + to_string(playerID) + " on nonexistent team " + to_string(teamID) + "\n");
		return NULL;
	}

	Player* p = t->getPlayerByID(playerID);
	if (t == NULL) {
		log("SERVER ERR: tried adding new program to nonexistent player " + to_string(playerID) + " on team " + to_string(teamID) + "\n");
		return NULL;
	}

	Program* pr = new Program(type);
	p->addProgram(pr);

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAM;
	m.teamID = teamID;
	m.playerID = playerID;
	m.programID = pr->getProgramID();
	m.progType = type;
	_server->sendMessageToAllClients(m);

	return pr;
}

SpawnGroup* Game::addSpawnGroup() {
	SpawnGroup* g = new SpawnGroup();
	spawnGroupList_->addFirst(g);

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_SPAWNGROUP;
	m.num = g->getGroupID();
	_server->sendMessageToAllClients(m);

	return g;
}

void Game::removeTeam(int teamID) {
	Team* t = getTeamByID(teamID);
	if (t == NULL) {
		log("SERVER ERR: tried to remove nonexistent team " + to_string(teamID) + "\n");
		return;
	}

	while (t->getAllPlayers()->getLength() > 0) {
		removePlayer(t->getAllPlayers()->getFirst()->getPlayerID(), teamID);
	}

	teamList_->remove(t);
	delete t;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_TEAMDELETE;
	m.teamID = teamID;
	_server->sendMessageToAllClients(m);
}

void Game::removePlayer(int playerID, int teamID) {
	Team* t = getTeamByID(teamID);
	if (t == NULL) {
		log("SERVER ERR: tried to remove player " + to_string(playerID) + " on nonexistent team " + to_string(teamID) + "\n");
		return;
	}

	Player* p = t->getPlayerByID(playerID);
	if (p == NULL) {
		log("SERVER ERR: tried to remove nonexistent player " + to_string(playerID) + " on team " + to_string(teamID) + "\n");
		return;
	}

	while (p->getProgList()->getLength() > 0) {
		removeProgram(p->getProgList()->getFirst()->getProgramID(), p->getPlayerID(), t->getTeamID());
	}

	t->getAllPlayers()->remove(p);
	delete p;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PLAYERDELETE;
	m.teamID = teamID;
	m.playerID = playerID;
	_server->sendMessageToAllClients(m);
}

void Game::removeProgram(int programID, int playerID, int teamID) {
	Team* t = getTeamByID(teamID);
	if (t == NULL) {
		log("SERVER ERR: tried to remove program " + to_string(programID) + " on player " + to_string(playerID) + " on nonexistent team " + to_string(teamID) + "\n");
		return;
	}

	Player* p = t->getPlayerByID(playerID);
	if (p == NULL) {
		log("SERVER ERR: tried to remove program " + to_string(programID) + " on nonexistent player " + to_string(playerID) + " on team " + to_string(teamID) + "\n");
		return;
	}

	Program* pr = p->getProgramByID(programID);
	if (pr == NULL) {
		log("SERVER ERR: tried to remove nonexistent program " + to_string(programID) + " on player " + to_string(playerID) + " on team " + to_string(teamID) + "\n");
		return;
	}

	while (pr->getTiles()->getLength() > 0) {
		pr->removeTile(pr->getTail());
	}

	p->getProgList()->remove(pr);
	delete pr;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAMDELETE;
	m.teamID = teamID;
	m.playerID = playerID;
	m.programID = programID;
	_server->sendMessageToAllClients(m);
}

void Game::removeSpawnGroup(int groupID) {
	SpawnGroup* g = NULL;
	Iterator<SpawnGroup*> it = spawnGroupList_->getIterator();
	while (it.hasNext()) {
		SpawnGroup* curr = it.next();
		if (curr->getGroupID() == groupID) {
			g = curr;
			break;
		}
	}

	if (g == NULL) {
		log("SERVER ERR: tried to remove nonexistent spawn group " + to_string(groupID) + "\n");
		return;
	}

	while (g->getTiles()->getLength() > 0)
		g->removeTile(*g->getTiles()->getFirst());

	spawnGroupList_->remove(g);
	delete g;

	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_SPAWNGROUPDELETE;
	m.num = groupID;
	_server->sendMessageToAllClients(m);
}

void Game::saveLevel(std::string fileName) {
	std::ofstream lvl;
	lvl.open("levels/" + fileName + ".urf", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!lvl.is_open()) {
		if (_debug >= DEBUG_MINIMAL)
			log("err opening file " + fileName + " for saving\n");
	} else {
		if (_debug >= DEBUG_MINIMAL)
			log("saving level + " + fileName + "...\n");

		// write the sizes of standard data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		if (_debug >= DEBUG_NORMAL)
			log("saving constants... int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) +
				" double:" + to_string(sizeOfDouble) + " bool:" + to_string(sizeOfBool) + "\n");
		lvl.write((char*)&sizeOfInt, 1);
		lvl.write((char*)&sizeOfChar, 1);
		lvl.write((char*)&sizeOfDouble, 1);
		lvl.write((char*)&sizeOfBool, 1);

		// write sizes of custom enum datatypes
		int8_t sizeOfItem = sizeof(ITEM);
		int8_t sizeOfTile = sizeof(TILE);
		int8_t sizeOfBkg = sizeof(BACKGROUND);
		int8_t sizeOfProg = sizeof(PROGRAM);
		if (_debug >= DEBUG_NORMAL)
			log("saving enums... item:" + to_string(sizeOfItem) + " tile:" + to_string(sizeOfTile) +
				" bkg:" + to_string(sizeOfBkg) + " prog:" + to_string(sizeOfProg) + "\n");
		lvl.write((char*)&sizeOfItem, 1);
		lvl.write((char*)&sizeOfTile, 1);
		lvl.write((char*)&sizeOfBkg, 1);
		lvl.write((char*)&sizeOfProg, 1);

		// write the size of the game grid to the file
		if (_debug >= DEBUG_NORMAL)
			log("saving grid bounds... left" + to_string(gridLeftBound_) + " right:" + to_string(gridRightBound_) +
				" top:" + to_string(gridTopBound_) + " bottom:" + to_string(gridBottomBound_) + "\n");
		lvl.write((char*)&gridLeftBound_, sizeOfInt);
		lvl.write((char*)&gridRightBound_, sizeOfInt);
		lvl.write((char*)&gridTopBound_, sizeOfInt);
		lvl.write((char*)&gridBottomBound_, sizeOfInt);

		// write the enum of the level's background
		lvl.write((char*)&bkg_, sizeOfBkg);

		// write tiles and items to file
		if (_debug >= DEBUG_NORMAL)
			log("saving tiles and items...\n");
		for (int x = gridLeftBound_; x < gridRightBound_; x++) {
			for (int y = gridTopBound_; y < gridBottomBound_; y++) {
				lvl.write((char*)(&(gridTiles_[x][y])), sizeOfTile);
				lvl.write((char*)(&(gridItems_[x][y])), sizeOfItem);
			}
		}

		// write the number of spawn groups to file
		int numSpawnGroups = spawnGroupList_->getLength();
		lvl.write((char*)&numSpawnGroups, sizeOfInt);
		if (_debug >= DEBUG_NORMAL)
			log("saving " + to_string(numSpawnGroups) + " spawn groups...\n");

		// for each spawn group
		Iterator<SpawnGroup*> groupIt = spawnGroupList_->getIterator();
		while (groupIt.hasNext()) {
			SpawnGroup* currGroup = groupIt.next();

			// write the number of coords in this spawngroup
			int numSpawnTiles = currGroup->getTiles()->getLength();
			lvl.write((char*)&numSpawnTiles, sizeOfInt);

			// write each tile coord in this spawngroup
			Iterator<Coord*> coordIt = currGroup->getTiles()->getIterator();
			while (coordIt.hasNext()) {
				Coord* currCoord = coordIt.next();

				// write these coords
				int xCoord = currCoord->x;
				lvl.write((char*)&xCoord, sizeOfInt);
				int yCoord = currCoord->y;
				lvl.write((char*)&yCoord, sizeOfInt);
			}
		}

		// write debug delineation
		int delineator = -1; // 0xFFFFFFFF - easy to see in hexdump
		lvl.write((char*)&delineator, sizeOfInt);

		// write number of teams
		int numTeams = teamList_->getLength();
		lvl.write((char*)&numTeams, sizeOfInt);
		if (_debug >= DEBUG_NORMAL)
			log("saving " + to_string(numTeams) + " teams...\n");

		// for each team
		Iterator<Team*> teamIt = teamList_->getIterator();
		while (teamIt.hasNext()) {
			Team* currTeam = teamIt.next();

			// write whether this team is a default human or AI team
			bool isDefaultHuman = (currTeam->isDefaultHuman() || currTeam->getTeamID() == 0) ? true : false;
			lvl.write((char*)&isDefaultHuman, sizeOfBool);
			bool isDefaultAI = (currTeam->isDefaultAI() || currTeam->getTeamID() == 1) ? true : false;
			lvl.write((char*)&isDefaultAI, sizeOfBool);

			// write the number of players on this team
			int numPlayers = currTeam->getAllPlayers()->getLength();
			lvl.write((char*)&numPlayers, sizeOfInt);
			if (_debug >= DEBUG_NORMAL)
				log("saving " + to_string(numPlayers) + " players on team " + to_string(currTeam->getTeamID()) + "...\n");

			// for each player
			Iterator<Player*> playerIt = currTeam->getAllPlayers()->getIterator();
			while (playerIt.hasNext()) {
				Player* currPlayer = playerIt.next();

				// write whether this player has an AI mind
				bool hasMind = (currPlayer->getMind() == NULL) ? false : true;
				lvl.write((char*)&hasMind, sizeOfBool);

				// write the number of programs that this player has
				int numPrograms = currPlayer->getProgList()->getLength();
				lvl.write((char*)&numPrograms, sizeOfInt);
				if (_debug >= DEBUG_NORMAL)
					log("saving " + to_string(numPrograms) + " programs owned by player " + to_string(currPlayer->getPlayerID()) + "...\n");

				// for each program
				Iterator<Program*> progIt = currPlayer->getProgList()->getIterator();
				while (progIt.hasNext()) {
					Program* currProg = progIt.next();

					// write program properties
					PROGRAM type = currProg->getType();
					lvl.write((char*)(&type), sizeOfProg);
					int maxHealth = currProg->getMaxHealth();
					lvl.write((char*)(&maxHealth), sizeOfInt);
					int maxMoves = currProg->getMaxMoves();
					lvl.write((char*)(&maxMoves), sizeOfInt);

					// write the number of tiles this program owns
					int numOwnedTiles = currProg->getTiles()->getLength();
					lvl.write((char*)&numOwnedTiles, sizeOfInt);
					if (_debug >= DEBUG_NORMAL)
						log("saving " + to_string(numOwnedTiles) + " tiles owned by program " + to_string(currProg->getProgramID()) + "...\n");

					// for each owned tile
					Iterator<Coord*> coordIt = currProg->getTiles()->getIterator();
					while (coordIt.hasNext()) {
						Coord* currCoord = coordIt.next();

						// write these coords
						int xCoord = currCoord->x;
						lvl.write((char*)&xCoord, sizeOfInt);
						int yCoord = currCoord->y;
						lvl.write((char*)&yCoord, sizeOfInt);
					}
				}
			}
		}

		// flush and close the file
		if (_debug >= DEBUG_MINIMAL)
			log("flushing and closing save file... ");
		lvl.flush();
		lvl.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");
	}
}

void Game::loadLevel(std::string str) {
	std::ifstream lvl;
	if (str.size() == 0) {
		lvl.open("levels/default.urf", std::ios::in | std::ios::binary);
	} else {
		lvl.open(str, std::ios::in | std::ios::binary);
	}

	if (!lvl.is_open()) {
		if (_debug >= DEBUG_MINIMAL)
			log("err opening level " + str + "\n");
	} else {
		if (_debug >= DEBUG_MINIMAL)
			log("loading level " + str + "...\n");

		// read the sizes of standard data types
		int8_t sizeOfInt;
		lvl.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		lvl.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		lvl.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		lvl.read((char*)&sizeOfBool, 1);
		if (_debug >= DEBUG_NORMAL)
			log("loaded constants... int:" + to_string(sizeOfInt) + " char:" + to_string(sizeOfChar) +
				" double:" + to_string(sizeOfDouble) + " bool:" + to_string(sizeOfBool) + "\n");

		// write sizes of custom enum datatypes
		int8_t sizeOfItem, sizeOfTile, sizeOfBkg, sizeOfProg;
		lvl.read((char*)&sizeOfItem, 1);
		lvl.read((char*)&sizeOfTile, 1);
		lvl.read((char*)&sizeOfBkg, 1);
		lvl.read((char*)&sizeOfProg, 1);
		if (_debug >= DEBUG_NORMAL)
			log("loaded enums: item:" + to_string(sizeOfItem) + " tile:" + to_string(sizeOfTile) +
				" bkg:" + to_string(sizeOfBkg) + " prog:" + to_string(sizeOfProg) + "\n");

		// load the size of the game grid
		int left, right, top, bottom;
		lvl.read((char*)&left, sizeOfInt);
		lvl.read((char*)&right, sizeOfInt);
		lvl.read((char*)&top, sizeOfInt);
		lvl.read((char*)&bottom, sizeOfInt);
		if (_debug >= DEBUG_NORMAL)
			log("loaded grid bounds... left" + to_string(left) + " right:" + to_string(right) +
				" top:" + to_string(top) + " bottom:" + to_string(bottom) + "\n");

		// load the enum of the level's background
		BACKGROUND bkg;
		lvl.read((char*)&bkg, sizeOfBkg);
		setBackground(bkg);

		// load tiles and items from the file
		if (_debug >= DEBUG_NORMAL)
			log("loading tiles and items...\n");
		for (int x = left; x < right; x++) {
			for (int y = top; y < bottom; y++) {
				// tiles
				TILE typ;
				lvl.read((char*)(&typ), sizeOfTile);
				setTileAt({ x, y }, typ);

				// items
				ITEM itm;
				lvl.read((char*)(&itm), sizeOfItem);
				setItemAt({ x,y }, itm);
			}
		}

		// load the number of spawn groups from file
		int numSpawnGroups;
		lvl.read((char*)&numSpawnGroups, sizeOfInt);
		if (_debug >= DEBUG_NORMAL)
			log("loading " + to_string(numSpawnGroups) + " spawn groups...\n");

		// for each spawn group
		for (int groupCounter = 0; groupCounter < numSpawnGroups; groupCounter++) {
			SpawnGroup* currGroup = addSpawnGroup();

			// read the number of coords in this spawngroup
			int numSpawnTiles;
			lvl.read((char*)&numSpawnTiles, sizeOfInt);

			// read each tile coord in this spawngroup
			for (int coordCounter = 0; coordCounter < numSpawnTiles; coordCounter++) {
				// read the coord
				Coord c;
				lvl.read((char*)&c.x, sizeOfInt);
				lvl.read((char*)&c.y, sizeOfInt);
				currGroup->addTile(c);
			}
		}

		// load (waste) debug delineation
		int delineator = -1; // 0xFFFFFFFF - easy to see in hexdump
		lvl.read((char*)&delineator, sizeOfInt);

		// load number of teams
		int numTeams;
		lvl.read((char*)&numTeams, sizeOfInt);
		if (_debug >= DEBUG_NORMAL)
			log("loading " + to_string(numTeams) + " teams...\n");

		// for each team
		for (int teamCount = 0; teamCount < numTeams; teamCount++) {
			Team* currTeam = addTeam();

			// load whether this team is a default human or AI team
			bool isDefaultHuman, isDefaultAI;
			lvl.read((char*)&isDefaultHuman, sizeOfBool);
			lvl.read((char*)&isDefaultAI, sizeOfBool);
			currTeam->setDefaultHuman(isDefaultHuman);
			currTeam->setDefaultAI(isDefaultAI);

			// load the number of players on this team
			int numPlayers;
			lvl.read((char*)&numPlayers, sizeOfInt);
			if (_debug >= DEBUG_NORMAL)
				log("loading " + to_string(numPlayers) + " players on team " + to_string(currTeam->getTeamID()) + "...\n");

			// for each player
			for (int playerCount = 0; playerCount < numPlayers; playerCount++) {
				Player* currPlayer = addPlayer(currTeam->getTeamID());

				// load whether this player has an AI mind
				bool hasMind;
				lvl.read((char*)&hasMind, sizeOfBool);
				if (hasMind)
					currPlayer->setMind(new AIBasic(currPlayer));

				// load the number of programs that this player has
				int numPrograms;
				lvl.read((char*)&numPrograms, sizeOfInt);
				if (_debug >= DEBUG_NORMAL)
					log("loading " + to_string(numPrograms) + " programs owned by player " + to_string(currPlayer->getPlayerID()) + "...\n");

				// for each program
				for (int programCount = 0; programCount < numPrograms; programCount++) {
					// load program properties
					PROGRAM type;
					int maxHealth, maxMoves;
					lvl.read((char*)(&type), sizeOfProg);
					lvl.read((char*)(&maxHealth), sizeOfInt);
					lvl.read((char*)(&maxMoves), sizeOfInt);

					Program* currProg = addProgram(type, currPlayer->getPlayerID(), currTeam->getTeamID());

					// load the number of tiles this program owns
					int numOwnedTiles;
					lvl.read((char*)&numOwnedTiles, sizeOfInt);
					if (_debug >= DEBUG_NORMAL)
						log("loading " + to_string(numOwnedTiles) + " tiles owned by program " + to_string(currProg->getProgramID()) + "...\n");

					// for each owned tile
					for (int tileCount = 0; tileCount < numOwnedTiles; tileCount++) {
						// load coords
						Coord c;
						lvl.read((char*)&c.x, sizeOfInt);
						lvl.read((char*)&c.y, sizeOfInt);
						currProg->addTail(c);
					}
				}
			}
		}

		// close the file
		lvl.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");

		// set the gamestatus to pregame
		setStatus(GAMESTATUS_PREGAME);
	}
}