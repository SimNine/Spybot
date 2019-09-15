#include "Node.h"
#include "Global.h"

#include <stdio.h>
#include <stdlib.h>

// constructor for procedurally generated nodes
Node::Node(int xp, int yp)
{
    x = xp;
    y = yp;
    nodeType = (rand()%7 + 1);
    zone = 0;
    nodeStatus = HIDDEN;
    children = NULL;
    img = dataContainer->node_normal[nodeType];
    levelStr = "custom";
}

// constructor for predetermined nodes (STORES)
Node::Node(int xp, int yp, int type, int nodeZone)
{
    x = xp;
    y = yp;
    nodeType = type;
    zone = nodeZone;
    nodeStatus = HIDDEN;
    children = NULL;
    img = dataContainer->node_normal[nodeType];
    levelStr = "undefined";
}

// constructor for predetermined nodes (*NOT* STORES)
Node::Node(int xp, int yp, int type, int nodeZone, std::string lvlFileName)
{
    x = xp;
    y = yp;
    nodeType = type;
    zone = nodeZone;
    nodeStatus = HIDDEN;
    children = NULL;
    img = dataContainer->node_normal[nodeType];
    levelStr = lvlFileName;
}

Node::~Node()
{
    //dtor
}

void Node::draw(int mapRootX, int mapRootY)
{
    // if this node is not available yet
    if (nodeStatus == HIDDEN)
    {
        return;
    }

    // if the mouse is over this node
    if (!isMouseOver(mapRootX, mapRootY))
    {
        if (nodeStatus == OWNED)
        {
            img = dataContainer->node_owned[nodeType];
        }
        else
        {
            img = dataContainer->node_normal[nodeType];
        }
    }
    else
    {
        if (nodeStatus == OWNED)
        {
            img = dataContainer->node_owned_over[nodeType];
        }
        else
        {
            img = dataContainer->node_normal_over[nodeType];
        }
    }

    // if this node is hightlighted
    if (nodeStatus == NOT_OWNED_SELECTED)
    {
        img = dataContainer->node_normal_selected[nodeType];
    }
    else if (nodeStatus == OWNED_SELECTED)
    {
        img = dataContainer->node_owned_selected[nodeType];
    }

    // get the width and height of the node image
    SDL_Rect destRect;
    int width, height;
    SDL_QueryTexture(img, NULL, NULL, &width, &height);

    // dump the dimensions and position into a rect
    destRect.x = this->x - mapRootX - width/2;
    destRect.y = this->y - mapRootY - dataContainer->node_yoffset[this->nodeType];
    destRect.w = width;
    destRect.h = height;

    // draw node icon
    SDL_RenderCopy(gRenderer, img, NULL, &destRect);

    // if debugging, draw bounding box
    if (debug)
    {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
        SDL_RenderDrawRect(gRenderer, &destRect);

        int xCenter = destRect.x + width/2;
        int yCenter = destRect.y + dataContainer->node_yoffset[this->nodeType];
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
        SDL_RenderDrawLine(gRenderer, xCenter-5, yCenter, xCenter+5, yCenter);
        SDL_RenderDrawLine(gRenderer, xCenter, yCenter-5, xCenter, yCenter+5);
    }
}

int Node::getX()
{
    return x;
}

int Node::getY()
{
    return y;
}

bool Node::click()
{
    if (nodeStatus == HIDDEN)
    {
        return false;
    }
    else if (nodeStatus == NOT_OWNED)
    {
        nodeStatus = NOT_OWNED_SELECTED;
    }
    else if (nodeStatus == OWNED)
    {
        nodeStatus = OWNED_SELECTED;
    }
    return false;
}

bool Node::isMouseOver(int mapRootX, int mapRootY)
{
    // get the width and height of the current node
    int width, height;
    SDL_QueryTexture(img, NULL, NULL, &width, &height);
    int widthOffset = width/2;
    int heightOffset = dataContainer->node_yoffset[this->nodeType];

    return (mousePosX > this->x - mapRootX - widthOffset &&
            mousePosX < this->x + widthOffset - mapRootX &&
            mousePosY > this->y - mapRootY - heightOffset &&
            mousePosY < this->y + (height - heightOffset) - mapRootY);
}

void Node::addChild(Node* child)
{
    if (children == NULL)
    {
        children = new LinkedList<Node*>(child);
    }
    else
    {
        LL_addObject(children, child);
    }
}

void Node::setNodeStatus(NodeStatus ns)
{
    nodeStatus = ns;
}

NodeStatus Node::getNodeStatus()
{
    return nodeStatus;
}

void Node::winNode()
{
    nodeStatus = OWNED;
    LinkedList<Node*>* curr = children;
    while (curr != NULL)
    {
        curr->getContents()->setNodeStatus(NOT_OWNED);
        curr = curr->getNext();
    }
}

std::string Node::getLevelStr()
{
    return levelStr;
}
