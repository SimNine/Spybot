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

Game::Game(bool serverSide) {
	serverSide_ = serverSide;
	teamList_ = new LinkedList<Team*>();
	initBoard();
	status_ = GAMESTATUS_PREGAME;
}

Game::Game(bool serverSide, std::string lvlStr) {
	serverSide_ = serverSide;
	teamList_ = new LinkedList<Team*>();
	initBoard();
	loadLevel(lvlStr);
	status_ = GAMESTATUS_PREGAME;
}

Game::~Game() {
	while (teamList_->getLength() > 0)
		delete teamList_->poll();
	delete teamList_;
}

void Game::initBoard() {
	// initialize the board
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

void Game::setTileAt(Coord pos, TILE t) {
	// check for OOB
	if (isOOB(pos))
		return;

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

void Game::saveLevel() {
	std::ofstream lvl;
	lvl.open("levels/default.urf", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!lvl.is_open()) {
		if (_debug >= DEBUG_MINIMAL) printf("err opening file for saving\n");
	} else {
		if (_debug >= DEBUG_MINIMAL) printf("saving level...\n");

		// begin by writing the sizes of various data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		if (_debug >= DEBUG_NORMAL) printf("saving constants... int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);
		lvl.write((char*)&sizeOfInt, 1);
		lvl.write((char*)&sizeOfChar, 1);
		lvl.write((char*)&sizeOfDouble, 1);
		lvl.write((char*)&sizeOfBool, 1);

		// write the size of the game grid to the file
		if (_debug >= DEBUG_NORMAL) printf("saving grid bounds... left:%i, right:%i, top:%i, bottom:%i\n", gridLeftBound_, gridRightBound_, gridTopBound_, gridBottomBound_);
		lvl.write((char*)&gridLeftBound_, sizeOfInt);
		lvl.write((char*)&gridRightBound_, sizeOfInt);
		lvl.write((char*)&gridTopBound_, sizeOfInt);
		lvl.write((char*)&gridBottomBound_, sizeOfInt);

		// write the enum of the level's background
		lvl.write((char*)&bkg_, sizeOfInt);

		// collect all the programs in a linked list
		if (_debug >= DEBUG_NORMAL) printf("gathering program list...\n");
		LinkedList<Program*> progs = LinkedList<Program*>();
		for (int x = gridLeftBound_; x < gridRightBound_; x++) {
			for (int y = gridTopBound_; y < gridBottomBound_; y++) {
				if (gridPrograms_[x][y] == NULL) {
					continue;
				}
				if (progs.contains(gridPrograms_[x][y])) {
					if (_debug >= DEBUG_NORMAL) printf("saving program\n");
					progs.addLast(gridPrograms_[x][y]);
				}
			}
		}

		// write all programs to the file
		int numPrograms = progs.getLength();
		if (_debug >= DEBUG_NORMAL) printf("saving %i programs...\n", numPrograms);
		lvl.write((char*)(&numPrograms), sizeOfInt);
		for (int i = 0; i < progs.getLength(); i++) {
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
		if (_debug >= DEBUG_NORMAL) printf("saving tiles, items, and program pointers...\n");
		for (int x = gridLeftBound_; x < gridRightBound_; x++) {
			for (int y = gridTopBound_; y < gridBottomBound_; y++) {
				lvl.write((char*)(&(gridTiles_[x][y])), sizeOfInt);
				lvl.write((char*)(&(gridItems_[x][y])), sizeOfInt);
				int index = progs.getIndexOf(gridPrograms_[x][y]);
				lvl.write((char*)(&index), sizeOfInt);
			}
		}

		// flush and close the file
		if (_debug >= DEBUG_MINIMAL)
			printf("flushing and closing save file... ");
		lvl.flush();
		lvl.close();
		if (_debug >= DEBUG_MINIMAL)
			printf("done\n");
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
			printf("err opening level %s\n", str.c_str());
	} else {
		if (_debug >= DEBUG_MINIMAL)
			printf("loading level %s...\n", str.c_str());

		// read the sizes of various data types
		if (_debug >= DEBUG_NORMAL) printf("loading constants...\n");
		int8_t sizeOfInt;
		lvl.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		lvl.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		lvl.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		lvl.read((char*)&sizeOfBool, 1);

		// load the size of the game grid
		if (_debug >= DEBUG_NORMAL) printf("loading grid bounds...\n");
		int left, right, top, bottom;
		lvl.read((char*)&left, sizeOfInt);
		lvl.read((char*)&right, sizeOfInt);
		lvl.read((char*)&top, sizeOfInt);
		lvl.read((char*)&bottom, sizeOfInt);

		// load the enum of the level's background
		lvl.read((char*)&bkg_, sizeOfInt);
		setBackground(bkg_);

		// load the list of programs
		int numPrograms;
		lvl.read((char*)(&numPrograms), sizeOfInt);
		if (_debug >= DEBUG_NORMAL) printf("loading %i programs...\n", numPrograms);
		LinkedList<Program*> progs = LinkedList<Program*>();
		for (int i = 0; i < numPrograms; i++) {
			int xHead, yHead, type, health, maxHealth, maxMoves, team;
			lvl.read((char*)(&xHead), sizeOfInt);
			lvl.read((char*)(&yHead), sizeOfInt);
			lvl.read((char*)(&type), sizeOfInt);
			lvl.read((char*)(&health), sizeOfInt);
			lvl.read((char*)(&maxHealth), sizeOfInt);
			lvl.read((char*)(&maxMoves), sizeOfInt);
			lvl.read((char*)(&team), sizeOfInt);
			Program* p = new Program((PROGRAM)type, team, { xHead, yHead });
			p->setProgramID(i);

			p->setMaxHealth(maxHealth);
			p->setMaxMoves(maxMoves);

			// create this program's team if it doesn't exist
			Team* t = getTeamByNum(team);
			if (t == NULL) {
				t = new Team(team);
				teamList_->addFirst(t);
			}

			// create this program's player if it doesn't exist
			Player* pl = t->getAllPlayers()->getFirst();
			if (pl == NULL) {
				pl = new Player(this, team);
				pl->setPlayerID(randInt());
				t->getAllPlayers()->addFirst(pl);

				// for now, the default AI team is team 1, so add a mind if team == 1
				if (team == 1)
					pl->setMind(new AIBasic(pl));
			}

			// add this program to this player
			pl->addProgram(p);

			progs.addLast(p);
		}

		// load the grid from the file
		if (_debug >= DEBUG_NORMAL) printf("loading tiles, items, and program pointers...\n");
		for (int x = left; x < right; x++) {
			for (int y = top; y < bottom; y++) {
				// tiles
				TILE typ;
				lvl.read((char*)(&typ), sizeOfInt);
				setTileAt({ x, y }, typ);

				// items
				ITEM itm;
				lvl.read((char*)(&itm), sizeOfInt);
				gridItems_[x][y] = itm;

				// programs
				int indx;
				lvl.read((char*)(&indx), sizeOfInt);
				gridPrograms_[x][y] = progs.getObjectAt(indx);
				if (gridPrograms_[x][y] != NULL) gridPrograms_[x][y]->addTail({ x, y });
			}
		}

		// close the file
		lvl.close();
		if (_debug >= DEBUG_MINIMAL)
			printf("done\n");
	}
}

void Game::removeReferencesToProgram(Program* p) {
	if (p == NULL)
		return;

	for (int i = 0; i < 200; i++)
		for (int j = 0; j < 200; j++)
			if (gridPrograms_[i][j] == p)
				gridPrograms_[i][j] = NULL;
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
}

BACKGROUND Game::getBackground() {
	return bkg_;
}

void Game::setProgramAt(Coord pos, Program* p) {
	// sanity check
	if (isOOB(pos))
		return;

	// place the program
	gridPrograms_[pos.x][pos.y] = p;
}

void Game::setItemAt(Coord pos, ITEM i) {
	// don't place an item if there's no tile here
	if (!isTiled(pos))
		return;

	// place the item here
	gridItems_[pos.x][pos.y] = i;
}

TILE Game::getTileAt(Coord pos) {
	if (pos.x >= 200 || pos.x < 0 || pos.y >= 200 || pos.y < 0)
		return TILE_NONE;
	return gridTiles_[pos.x][pos.y];
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
	case GAMESTATUS_PREGAME:
		break;
	case GAMESTATUS_PLAYING:
		for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
			if (getTileAt({ x, y }) == TILE_SPAWN || getTileAt({ x, y }) == TILE_SPAWN2) {
				setTileAt({ x, y }, TILE_PLAIN);

				if (serverSide_) {
					// convert all spawntiles to plain tiles
					Message msg;
					msg.type = MSGTYPE_INFO;
					msg.infoType = MSGINFOTYPE_TILE;
					msg.tileType = TILE_PLAIN;
					msg.pos = Coord{ x, y };
					_server->sendMessageToAllClients(msg);

					// send the current player turn to each client
					msg.type = MSGTYPE_NEXTTURN;
					msg.clientID = 0;
					msg.playerID = teamList_->getFirst()->getAllPlayers()->getFirst()->getPlayerID();
					_server->sendMessageToAllClients(msg);

					// set the turn to be the first player
					currTurnPlayer_ = getPlayerByID(msg.playerID);
				}
			}
		break;
	case GAMESTATUS_END:
		break;
	}

	status_ = g;

	if (serverSide_) {
		Message m;
		m.type = MSGTYPE_INFO;
		m.infoType = MSGINFOTYPE_GAMESTATUS;
		m.statusType = g;
		_server->sendMessageToAllClients(m);
	}
}

void Game::moveProgramTo(Program* p, Coord c) {
	p->moveTo(c);
	setProgramAt(c, p);
}

Player* Game::getCurrTurnPlayer() {
	return currTurnPlayer_;
}

void Game::setCurrTurnPlayer(Player* p) {
	currTurnPlayer_ = p;

	if (serverSide_) {
		// send a message saying it's this player's turn
		Message m;
		m.type = MSGTYPE_NEXTTURN;
		m.clientID = 0;
		m.playerID = p->getPlayerID();
		_server->sendMessageToAllClients(m);
	}
}

LinkedList<Team*>* Game::getAllTeams() {
	return teamList_;
}

Team* Game::getTeamByNum(int teamNum) {
	Iterator<Team*> it = teamList_->getIterator();
	while (it.hasNext()) {
		Team* curr = it.next();
		if (curr->getTeamNum() == teamNum)
			return curr;
	}

	return NULL;
}

Player* Game::getFollowingPlayer(Player* currPlayer) {
	Team* currTeam = getTeamByNum(currPlayer->getTeam());

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

bool Game::isServerSide() {
	return serverSide_;
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
					currTeamWinning = currTeam->getTeamNum();
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
		m.team = currTeamWinning;
		_server->sendMessageToAllClients(m);
		printf("SERVER: game detected winning condition for team %i\n", currTeamWinning);
	}
}