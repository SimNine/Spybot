#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Node {
public:
	Node(Coord pos, int type, int zone, int id);
	virtual ~Node();
	void draw(Coord);
	Coord getPos();
	bool mouseDown();
	bool isMouseOver(Coord);
	void addNeighbor(Node*);
	void setNodeStatus(NODESTATUS);
	NODESTATUS getNodeStatus();
	void winNode();
	int getLevelId();
	int getNodeType();
	int getZone();
	LinkedList<Node*>* getNeighbors();
protected:
private:
	Coord pos_;
	int nodeType_;
	NODESTATUS nodeStatus_;
	int zone_;
	int id_;
	SDL_Texture* img_;
	LinkedList<Node*>* neighbors_;
};