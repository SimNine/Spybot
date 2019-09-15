#ifndef NODEICON_H
#define NODEICON_H

#include <SDL.h>
#include <string>

#include "LinkedList.h"
#include "Enums.h"

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
        bool mouseDown();
        bool isMouseOver(int, int);
        void addChild(Node*);
        void setNodeStatus(NODESTATUS);
        NODESTATUS getNodeStatus();
        void winNode();
        std::string getLevelStr();
    protected:
    private:
        int x;
        int y;
        int nodeType;
        NODESTATUS nodeStatus;
        SDL_Texture* img;
        LinkedList<Node*>* children;
        int zone;
        std::string levelStr;
};

#endif // NODEICON_H
