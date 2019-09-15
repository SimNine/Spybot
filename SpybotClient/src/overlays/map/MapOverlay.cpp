#include "Standard.h"
#include "MapOverlay.h"

#include "Global.h"
#include "Data.h"
#include "GUIButton.h"
#include "Message.h"
#include "Node.h"
#include "Client.h"
#include "ProgramInventoryDisplay.h"
#include "MainOverlay.h"
#include "ResourceLoader.h"
#include "ConnectionManager.h"
#include "BackgroundOverlay.h"
#include "Main.h"
#include "GUITexture.h"

MapOverlay::MapOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_clear) {
	// check to make sure default maps are generated
	nodeList_ = new LinkedList<Node*>();
	loadMap("levels/classic.urf");
	if (nodeList_->getLength() == 0)
		generateDefaultMaps();

	isAnimOccurring_ = false;
	selectedNode_ = NULL;
	bkgX_ = 200;
	bkgY_ = 500;
	shiftSpeed_ = 0.25;

	// create level confirm / cancel dialog box
	levelConfirm_ = new GUIContainer(this, ANCHOR_NORTHWEST, { 20, 20 }, { 252, 194 }, _color_bkg_standard);
	levelConfirm_->setTransparency(0);
	GUIButton* battleButton = new GUIButton(levelConfirm_, ANCHOR_NORTHWEST, { 130, 169 }, { 115, 14 },
		[] () {
		if (_mapOverlay->getSelectedNode()->getNodeType() != 7) {
			Message msg;
			msg.type = MSGTYPE_GAMECONFIG;
			msg.gameConfigType = MSGGAMECONFIGTYPE_LEVEL_NUMBERED;
			msg.num = _mapOverlay->getSelectedNode()->getLevelId();
			_connectionManager->sendMessage(msg);

			msg.gameConfigType = MSGGAMECONFIGTYPE_CAMPAIGN;
			_connectionManager->sendMessage(msg);

			msg.type = MSGTYPE_STARTGAME;
			_connectionManager->sendMessage(msg);
		}
		_mapOverlay->clearSelectedNode();
	},
		_map_button_beginDatabattle_normal,
		_map_button_beginDatabattle_over);
	levelConfirm_->addObject(battleButton);
	GUIButton* cancelButton = new GUIButton(levelConfirm_, ANCHOR_NORTHWEST, { 7, 169 }, { 115, 14 },
		[] () {
		_mapOverlay->clearSelectedNode();
	},
		_map_button_cancel_normal,
		_map_button_cancel_over);
	levelConfirm_->addObject(cancelButton);

	this->addObject(levelConfirm_);

	// add inventory display object
	invDisplay_ = new ProgramInventoryDisplay(ANCHOR_NORTHEAST, { -20, 20 }, { 300, 500 }, this);
	this->addObject(invDisplay_);

	// add pause menu
	pauseMenu_ = new GUIContainer(NULL, ANCHOR_CENTER, { 0, 0 }, { _screenWidth, _screenHeight }, _color_bkg_standard);
	pauseMenu_->setTransparency(0);
	this->addObject(pauseMenu_);

	GUITexture* pauseText = new GUITexture(pauseMenu_, ANCHOR_CENTER, { -200, -200 }, "PAUSED", 150);
	pauseMenu_->addObject(pauseText);

	GUIContainer* pauseMenuOptions = new GUIContainer(pauseMenu_, ANCHOR_CENTER, { 0, 0 }, { 220, 3 * 60 + 10 }, _color_bkg_standard);
	pauseMenu_->addObject(pauseMenuOptions);
	GUIButton* resumeButton = new GUIButton(pauseMenuOptions, ANCHOR_NORTHWEST, { 10, 10 }, { 200, 50 },
		[] () {
		_mapOverlay->hidePauseMenu();
	}, _game_button_resume);
	pauseMenuOptions->addObject(resumeButton);
	GUIButton* exitToMainButton = new GUIButton(pauseMenuOptions, ANCHOR_NORTHWEST, { 10, 70 }, { 200, 50 },
		[] () {_overlayStack->removeAll();
	_overlayStack->push(_backgroundOverlay);
	_overlayStack->push(_mainOverlay);
	_mainOverlay->loginHide(0);
	_mapOverlay->hidePauseMenu();
	}, _game_button_quitToMain);
	pauseMenuOptions->addObject(exitToMainButton);
	GUIButton* exitToDesktopButton = new GUIButton(pauseMenuOptions, ANCHOR_NORTHWEST, { 10, 130 }, { 200, 50 },
		[] () {_quit = true; },
		_game_button_quitToDesktop);
	pauseMenuOptions->addObject(exitToDesktopButton);
}

MapOverlay::~MapOverlay() {
	//dtor
}

void MapOverlay::shiftBkg(double x, double y) {
	int bkgWidth;
	int bkgHeight;
	SDL_QueryTexture(bkgImg_, NULL, NULL, &bkgWidth, &bkgHeight);

	if (bkgX_ + x < 0)
		bkgX_ = 0;
	else if (bkgX_ + x + _screenWidth > bkgWidth)
		bkgX_ = bkgWidth - _screenWidth;
	else
		bkgX_ += x;

	if (bkgY_ + y < 0)
		bkgY_ = 0;
	else if (bkgY_ + y + _screenHeight > bkgHeight)
		bkgY_ = bkgHeight - _screenHeight;
	else
		bkgY_ += y;
}

void MapOverlay::drawBkg() {
	SDL_Rect bkgRect;
	bkgRect.x = (int)bkgX_;
	bkgRect.y = (int)bkgY_;
	bkgRect.w = _screenWidth;
	bkgRect.h = _screenHeight;

	// draw background image
	SDL_RenderCopy(_renderer, bkgImg_, &bkgRect, NULL);
}

void MapOverlay::drawNodes() {
	Iterator<Node*> it = nodeList_->getIterator();
	while (it.hasNext()) {
		Node* currIcon = it.next();
		if (currIcon->getPos().x > bkgX_ - 200 &&
			currIcon->getPos().y > bkgY_ - 200 &&
			currIcon->getPos().x < bkgX_ + _screenWidth + 200 &&
			currIcon->getPos().y < bkgY_ + _screenHeight + 200) {
			currIcon->draw({ (int)bkgX_, (int)bkgY_ });
		}
	}
}

void MapOverlay::draw() {
	drawBkg();
	drawNodes();
	GUIContainer::drawContents();
}

bool MapOverlay::mouseDown() {
	if (GUIContainer::mouseDown())
		return true;

	// spits out the location (on the map bkg) of this click
	if (_debug >= DEBUG_NORMAL)
		log(to_string((int)bkgX_ + _mousePos.x) + "," + to_string((int)bkgY_ + _mousePos.y) + "\n");

	// if mapscreen animation is occurring, don't take input
	if (selectedNode_ != NULL)
		return false;

	// try clicking on node icons
	bool ret = false;
	Iterator<Node*> it = nodeList_->getIterator();
	while (it.hasNext()) {
		Node* n = it.next();
		if (n->getNodeStatus() != NODESTATUS_HIDDEN && n->isMouseOver({ (int)bkgX_, (int)bkgY_ })) {
			ret = true;
			n->mouseDown();
			shiftTo(n);
		}
	}

	return ret;
}

void MapOverlay::shiftTo(Node* n) {
	// if mapscreen is already animating, don't do this shit
	if (selectedNode_ != NULL) {
		return;
	} else {
		isAnimOccurring_ = true;
		selectedNode_ = n;
	}
}

Node* MapOverlay::getSelectedNode() {
	return selectedNode_;
}

void MapOverlay::clearSelectedNode() {
	if (selectedNode_ == NULL)
		return;

	if (selectedNode_->getNodeStatus() == NODESTATUS_UNOWNED_SELECTED)
		selectedNode_->setNodeStatus(NODESTATUS_UNOWNED);
	else if (selectedNode_->getNodeStatus() == NODESTATUS_OWNED_SELECTED)
		selectedNode_->setNodeStatus(NODESTATUS_OWNED);

	levelConfirm_->setTransparency(0);
	selectedNode_ = NULL;
}

void MapOverlay::tick(int ms) {
	if (pauseMenu_->isVisible())
		return;

	// tick all GUIObjects
	GUIContainer::tick(ms);
	double shiftAmt = shiftSpeed_*ms;

	// if the mapscreen is currently animated, don't take shift input
	if (isAnimOccurring_ || selectedNode_ != NULL) {
		// get the width and height of the background
		int bkgWidth;
		int bkgHeight;
		SDL_QueryTexture(bkgImg_, NULL, NULL, &bkgWidth, &bkgHeight);

		// variables representing if the map has shifted sufficiently
		bool xShift = false;
		bool yShift = false;

		// shift on the x-axis
		int xDis = (int)selectedNode_->getPos().x - (int)bkgX_ - (int)(_screenWidth / 2);
		// if this node is left of center
		if (xDis < 0 && bkgX_ > 0) {
			if (xDis < -shiftAmt) shiftBkg(-shiftAmt, 0);
			else bkgX_ = selectedNode_->getPos().x - _screenWidth / 2;
		}
		// if the node is right of center
		else if (xDis > 0 && bkgX_ < bkgWidth - _screenWidth) {
			if (xDis > shiftAmt) shiftBkg(shiftAmt, 0);
			else bkgX_ = selectedNode_->getPos().x - _screenWidth / 2;
		} else {
			xShift = true;
		}

		// shift on the y-axis
		int yDis = (int)selectedNode_->getPos().y - (int)bkgY_ - (int)(_screenHeight / 2);
		// if this node is above center
		if (yDis < 0 && bkgY_ > 0) {
			if (yDis < -shiftAmt) shiftBkg(0, -shiftAmt);
			else bkgY_ = selectedNode_->getPos().y - _screenHeight / 2;
		}
		// if this node is below center
		else if (yDis > 0 && bkgY_ < bkgHeight - _screenHeight) {
			if (yDis > shiftAmt) shiftBkg(0, shiftAmt);
			else bkgY_ = selectedNode_->getPos().y - _screenHeight / 2;
		} else {
			yShift = true;
		}

		// if done shifting
		if (xShift && yShift) {
			isAnimOccurring_ = false;
			levelConfirm_->setTransparency(255);
		}
		return;
	} else {
		// scan for keys currently pressed
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP]) {
			shiftBkg(0, -shiftAmt);
		} else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
			shiftBkg(0, shiftAmt);
		}

		if (currentKeyStates[SDL_SCANCODE_LEFT]) {
			shiftBkg(-shiftAmt, 0);
		} else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
			shiftBkg(shiftAmt, 0);
		}

		// if the mouse is at an edge, try to shift the background
		if (_mousePos.x < 20) {
			shiftBkg(-shiftAmt, 0);
		} else if (_mousePos.x > _screenWidth - 20) {
			shiftBkg(shiftAmt, 0);
		}

		if (_mousePos.y < 20) {
			shiftBkg(0, -shiftAmt);
		} else if (_mousePos.y > _screenHeight - 20) {
			shiftBkg(0, shiftAmt);
		}
	}
}

void MapOverlay::updateProgramInvDisplay() {
	invDisplay_->updateContents();
}

void MapOverlay::toggleInvDisplay() {
	if (invDisplay_->isVisible()) 
		invDisplay_->setTransparency(0);
	else 
		invDisplay_->setTransparency(255);
}

void MapOverlay::unlockAllLevels() {
	nodeList_->forEach([] (Node* n) {n->winNode(); });
}

void MapOverlay::generateDefaultMaps() {
	// begin generating classic map
	if (nodeList_ == NULL)
		nodeList_ = new LinkedList<Node*>();
	else
		while (nodeList_->getLength() > 0)
			delete nodeList_->poll();

	// zone 1 nodes
	Node* baseNode = new Node({ 558, 881 }, 0, 1, 0);
	nodeList_->addLast(baseNode);
	Node* n1 = new Node({ 706, 882 }, 4, 1, 1);
	nodeList_->addLast(n1);
	baseNode->addNeighbor(n1);
	Node* n2 = new Node({ 462, 1025 }, 3, 1, 2);
	nodeList_->addLast(n2);
	baseNode->addNeighbor(n2);
	Node* n3 = new Node({ 464, 690 }, 7, 1, 3);
	nodeList_->addLast(n3);
	baseNode->addNeighbor(n3);
	Node* n5 = new Node({ 750, 786 }, 2, 1, 5);
	nodeList_->addLast(n5);
	n1->addNeighbor(n5);
	Node* n4 = new Node({ 607, 617 }, 2, 1, 4);
	nodeList_->addLast(n4);
	n5->addNeighbor(n4);
	Node* n6 = new Node({ 847, 667 }, 4, 1, 6);
	nodeList_->addLast(n6);
	n5->addNeighbor(n6);
	Node* n7 = new Node({ 800, 1025 }, 4, 1, 7);
	nodeList_->addLast(n7);
	n1->addNeighbor(n7);

	// zone 2 nodes
	Node* n8 = new Node({ 704, 666 }, 3, 2, 8);
	nodeList_->addLast(n8);
	n5->addNeighbor(n8);
	Node* n9 = new Node({ 749, 570 }, 4, 2, 9);
	nodeList_->addLast(n9);
	n8->addNeighbor(n9);
	Node* n10 = new Node({ 702, 1002 }, 3, 2, 10);
	nodeList_->addLast(n10);
	n2->addNeighbor(n10);
	Node* n11 = new Node({ 605, 1194 }, 2, 2, 11);
	nodeList_->addLast(n11);
	n10->addNeighbor(n11);
	Node* n12 = new Node({ 655, 1099 }, 7, 2, 12);
	nodeList_->addLast(n12);
	n11->addNeighbor(n12);
	Node* n13 = new Node({ 895, 1098 }, 4, 2, 13);
	nodeList_->addLast(n13);
	n10->addNeighbor(n13);
	Node* n14 = new Node({ 751, 1266 }, 3, 2, 14);
	nodeList_->addLast(n14);
	n13->addNeighbor(n14);
	Node* n15 = new Node({ 511, 1288 }, 3, 2, 15);
	nodeList_->addLast(n15);
	n14->addNeighbor(n15);
	Node* n16 = new Node({ 990, 978 }, 1, 2, 16);
	nodeList_->addLast(n16);
	n13->addNeighbor(n16);
	Node* n17 = new Node({ 1185, 882 }, 1, 2, 17);
	nodeList_->addLast(n17);
	n16->addNeighbor(n17);

	// zone 3 nodes
	Node* n18 = new Node({ 848, 450 }, 3, 3, 18);
	nodeList_->addLast(n18);
	n9->addNeighbor(n18);
	Node* n19 = new Node({ 1039, 425 }, 7, 3, 19);
	nodeList_->addLast(n19);
	n18->addNeighbor(n19);
	Node* n20 = new Node({ 989, 642 }, 5, 3, 20);
	nodeList_->addLast(n20);
	n18->addNeighbor(n20);
	Node* n21 = new Node({ 1089, 784 }, 4, 3, 21);
	nodeList_->addLast(n21);
	n20->addNeighbor(n21);
	Node* n22 = new Node({ 847, 809 }, 4, 3, 22);
	nodeList_->addLast(n22);
	n21->addNeighbor(n22);
	Node* n23 = new Node({ 1422, 931 }, 5, 3, 23);
	nodeList_->addLast(n23);
	n17->addNeighbor(n23);
	Node* n24 = new Node({ 1663, 954 }, 5, 3, 24);
	nodeList_->addLast(n24);
	n23->addNeighbor(n24);
	Node* n25 = new Node({ 1327, 785 }, 2, 3, 25);
	nodeList_->addLast(n25);
	n23->addNeighbor(n25);
	Node* n26 = new Node({ 1282, 689 }, 2, 3, 26);
	nodeList_->addLast(n26);
	n25->addNeighbor(n26);
	Node* n27 = new Node({ 1327, 1050 }, 1, 3, 27);
	nodeList_->addLast(n27);
	n17->addNeighbor(n27);
	Node* n28 = new Node({ 1566, 1074 }, 3, 3, 28);
	nodeList_->addLast(n28);
	n27->addNeighbor(n28);
	Node* n29 = new Node({ 1087, 1075 }, 1, 3, 29);
	nodeList_->addLast(n29);
	n27->addNeighbor(n29);
	Node* n30 = new Node({ 1232, 1240 }, 1, 3, 30);
	nodeList_->addLast(n30);
	n27->addNeighbor(n30);

	// zone 4 nodes
	Node* n31 = new Node({ 1471, 1218 }, 3, 4, 31);
	nodeList_->addLast(n31);
	n30->addNeighbor(n31);
	Node* n32 = new Node({ 1664, 1314 }, 7, 4, 32);
	nodeList_->addLast(n32);
	n31->addNeighbor(n32);
	Node* n34 = new Node({ 1807, 1051 }, 1, 4, 34);
	nodeList_->addLast(n34);
	n31->addNeighbor(n34);
	Node* n33 = new Node({ 1712, 1242 }, 3, 4, 33);
	nodeList_->addLast(n33);
	n34->addNeighbor(n33);
	Node* n35 = new Node({ 1902, 930 }, 5, 4, 35);
	nodeList_->addLast(n35);
	n34->addNeighbor(n35);
	Node* n36 = new Node({ 1712, 834 }, 4, 4, 36);
	nodeList_->addLast(n36);
	n35->addNeighbor(n36);
	Node* n37 = new Node({ 1570, 664 }, 5, 4, 37);
	nodeList_->addLast(n37);
	n36->addNeighbor(n37);
	Node* n38 = new Node({ 1424, 475 }, 5, 4, 38);
	nodeList_->addLast(n38);
	n37->addNeighbor(n38);
	Node* n39 = new Node({ 1352, 654 }, 5, 4, 39);
	nodeList_->addLast(n39);
	n38->addNeighbor(n39);
	Node* n40 = new Node({ 1232, 569 }, 4, 4, 40);
	nodeList_->addLast(n40);
	n38->addNeighbor(n40);

	// zone 5 nodes
	Node* n41 = new Node({ 1808, 642 }, 6, 5, 41);
	nodeList_->addLast(n41);
	n36->addNeighbor(n41);

	// save classic map
	saveMap("levels/classic.urf");
}

void MapOverlay::saveMap(std::string url) {
	std::ofstream m;
	m.open(url, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!m.is_open()) {
		if (_debug >= DEBUG_MINIMAL) 
			log("err opening file " + url + " for saving map\n");
	} else {
		if (_debug >= DEBUG_MINIMAL) 
			log("saving map " + url + "...\n");

		// begin by writing the sizes of various data types
		int8_t sizeOfInt = sizeof(int);
		int8_t sizeOfChar = sizeof(char);
		int8_t sizeOfDouble = sizeof(double);
		int8_t sizeOfBool = sizeof(bool);
		if (_debug >= DEBUG_NORMAL) 
			log("saving constants...\n");
		m.write((char*)&sizeOfInt, 1);
		m.write((char*)&sizeOfChar, 1);
		m.write((char*)&sizeOfDouble, 1);
		m.write((char*)&sizeOfBool, 1);

		// begin writing the map to file
		int numNodes = nodeList_->getLength();
		m.write((char*)&numNodes, sizeOfInt);
		if (_debug >= DEBUG_NORMAL) 
			log("saving " + to_string(numNodes) + " nodes...\n");

		// save each node's data
		for (int i = 0; i < numNodes; i++) {
			Node* curr = nodeList_->getObjectAt(i);
			int xCoord = curr->getPos().x;
			int yCoord = curr->getPos().y;
			int type = curr->getNodeType();
			int zone = curr->getZone();
			int id = curr->getLevelId();
			m.write((char*)&xCoord, sizeOfInt);
			m.write((char*)&yCoord, sizeOfInt);
			m.write((char*)&type, sizeOfInt);
			m.write((char*)&zone, sizeOfInt);
			m.write((char*)&id, sizeOfInt);
		}

		// save each node's children
		if (_debug >= DEBUG_NORMAL)
			log ("saving nodes' children...\n");
		for (int i = 0; i < numNodes; i++) {
			Node* curr = nodeList_->getObjectAt(i);
			int numChildren = curr->getNeighbors()->getLength();
			m.write((char*)&numChildren, sizeOfInt);

			for (int j = 0; j < numChildren; j++) {
				int child = nodeList_->getIndexOf(curr->getNeighbors()->getObjectAt(j));
				m.write((char*)&child, sizeOfInt);
			}
		}

		// flush and close the file
		if (_debug >= DEBUG_MINIMAL)
			log("flushing and closing map file... ");
		m.flush();
		m.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");
	}
}

void MapOverlay::loadMap(std::string url) {
	std::ifstream m;
	m.open(url, std::ios::in | std::ios::binary);
	if (!m.is_open()) {
		if (_debug >= DEBUG_MINIMAL)
			log("err opening map " + url + "\n");
	} else {
		if (_debug >= DEBUG_MINIMAL)
			log("loading map " + url + "...\n");

		// read the sizes of various data types
		if (_debug >= DEBUG_NORMAL)
			log("loading constants...\n");
		int8_t sizeOfInt;
		m.read((char*)&sizeOfInt, 1);
		int8_t sizeOfChar;
		m.read((char*)&sizeOfChar, 1);
		int8_t sizeOfDouble;
		m.read((char*)&sizeOfDouble, 1);
		int8_t sizeOfBool;
		m.read((char*)&sizeOfBool, 1);

		// clear previous nodes
		if (nodeList_ == NULL)
			nodeList_ = new LinkedList<Node*>();
		else
			while (nodeList_->getLength() > 0)
				delete nodeList_->poll();

		// begin reading map data
		int numNodes;
		m.read((char*)&numNodes, sizeOfInt);

		// build and add the nodes
		for (int i = 0; i < numNodes; i++) {
			int xCoord, yCoord, type, zone, id;
			m.read((char*)&xCoord, sizeOfInt);
			m.read((char*)&yCoord, sizeOfInt);
			m.read((char*)&type, sizeOfInt);
			m.read((char*)&zone, sizeOfInt);
			m.read((char*)&id, sizeOfInt);

			nodeList_->addLast(new Node({ xCoord, yCoord }, type, zone, id));
		}

		// add nodes' children
		for (int i = 0; i < numNodes; i++) {
			Node* curr = nodeList_->getObjectAt(i);

			int numChildren;
			m.read((char*)&numChildren, sizeOfInt);

			for (int j = 0; j < numChildren; j++) {
				int childIndex;
				m.read((char*)&childIndex, sizeOfInt);
				curr->addNeighbor(nodeList_->getObjectAt(childIndex));
			}
		}

		// unlock the first node
		nodeList_->getFirst()->winNode();

		// close the file
		m.close();
		if (_debug >= DEBUG_MINIMAL)
			log("done\n");
	}
}

void MapOverlay::switchMap(MAPPRESET pre) {
	switch (pre) {
	case MAPPRESET_CLASSIC:
		bkgImg_ = loadTexture("resources/map/map.png");
		loadMap("levels/classic.urf");
		break;
	case MAPPRESET_NIGHTFALL:
		bkgImg_ = loadTexture("resources/map/nightfall/map.png");
		loadMap("levels/nightfall.urf");
		break;
	case MAPPRESET_PROCEDURAL:
		log("placeholder: load procedural map\n");
		break;
	default:
		break;
	}
}

void MapOverlay::showPauseMenu() {
	pauseMenu_->setTransparency(255);
}

void MapOverlay::hidePauseMenu() {
	pauseMenu_->setTransparency(0);
}

void MapOverlay::winNode(int nodeID) {
	Iterator<Node*> it = nodeList_->getIterator();
	while (it.hasNext()) {
		Node* curr = it.next();
		if (curr->getLevelId() == nodeID) {
			curr->winNode();
			return;
		}
	}
}