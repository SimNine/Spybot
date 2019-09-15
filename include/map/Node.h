#ifndef NODEICON_H
#define NODEICON_H

#include <SDL.h>
#include <string>

#include "LinkedList.h"

enum NodeStatus { HIDDEN, NOT_OWNED, NOT_OWNED_SELECTED, OWNED, OWNED_SELECTED };

class Node
{
    public:
        Node(int, int);
        Node(int, int, int, int);
        Node(int, int, int, int, std::string);
        virtual ~Node();
        void draw(int, int);
        int getX();
        int getY();
        bool click();
        bool isMouseOver(int, int);
        void addChild(Node*);
        void setNodeStatus(NodeStatus);
        NodeStatus getNodeStatus();
        void winNode();
        std::string getLevelStr();
    protected:
    private:
        int x;
        int y;
        int nodeType;
        NodeStatus nodeStatus;
        SDL_Texture* img;
        LinkedList<Node*>* children;
        int zone;
        std::string levelStr;
};

#endif // NODEICON_H
