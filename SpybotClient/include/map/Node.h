#ifndef NODEICON_H
#define NODEICON_H

#include "Standard.h"
#include "LinkedList.h"

class Node
{
    public:
        Node(Coord, int, int, int);
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
        Coord pos;
        int nodeType;
        NODESTATUS nodeStatus;
        int zone;
        int id;
        SDL_Texture* img;
        LinkedList<Node*>* neighbors;
};

#endif // NODEICON_H
