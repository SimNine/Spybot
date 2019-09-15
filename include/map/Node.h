#ifndef NODEICON_H
#define NODEICON_H

#include <SDL.h>
#include <string>

#include "LinkedList.h"
#include "Enums.h"
#include "Coord.h"

class Node
{
    public:
        Node(Coord);
        Node(Coord, int, int);
        Node(Coord, int, int, std::string);
        virtual ~Node();
        void draw(Coord);
        Coord getPos();
        bool mouseDown();
        bool isMouseOver(Coord);
        void addChild(Node*);
        void setNodeStatus(NODESTATUS);
        NODESTATUS getNodeStatus();
        void winNode();
        std::string getLevelStr();
    protected:
    private:
        Coord pos;
        int nodeType;
        NODESTATUS nodeStatus;
        SDL_Texture* img;
        LinkedList<Node*>* children;
        int zone;
        std::string levelStr;
};

#endif // NODEICON_H
