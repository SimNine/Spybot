#include "Node.h"
#include "Global.h"

#include <stdio.h>
#include <stdlib.h>

// constructor for procedurally generated nodes
Node::Node(Coord p)
{
    pos = p;
    nodeType = (rand()%7 + 1);
    zone = 0;
    nodeStatus = NODESTATUS_HIDDEN;
    children = NULL;
    img = dataContainer->node_normal[nodeType];
    levelStr = "custom";
}

// constructor for predetermined nodes (STORES)
Node::Node(Coord p, int type, int nodeZone)
{
    pos = p;
    nodeType = type;
    zone = nodeZone;
    nodeStatus = NODESTATUS_HIDDEN;
    children = NULL;
    img = dataContainer->node_normal[nodeType];
    levelStr = "undefined";
}

// constructor for predetermined nodes (*NOT* STORES)
Node::Node(Coord p, int type, int nodeZone, std::string lvlFileName)
{
    pos = p;
    nodeType = type;
    zone = nodeZone;
    nodeStatus = NODESTATUS_HIDDEN;
    children = NULL;
    img = dataContainer->node_normal[nodeType];
    levelStr = lvlFileName;
}

Node::~Node()
{
    //dtor
}

void Node::draw(Coord mapRoot)
{
    // if this node is not available yet
    if (nodeStatus == NODESTATUS_HIDDEN)
    {
        return;
    }

    // if the mouse is over this node
    if (!isMouseOver(mapRoot))
    {
        if (nodeStatus == NODESTATUS_OWNED)
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
        if (nodeStatus == NODESTATUS_OWNED)
        {
            img = dataContainer->node_owned_over[nodeType];
        }
        else
        {
            img = dataContainer->node_normal_over[nodeType];
        }
    }

    // if this node is hightlighted
    if (nodeStatus == NODESTATUS_UNOWNED_SELECTED)
    {
        img = dataContainer->node_normal_selected[nodeType];
    }
    else if (nodeStatus == NODESTATUS_OWNED_SELECTED)
    {
        img = dataContainer->node_owned_selected[nodeType];
    }

    // get the width and height of the node image
    SDL_Rect destRect;
    int width, height;
    SDL_QueryTexture(img, NULL, NULL, &width, &height);

    // dump the dimensions and position into a rect
    destRect.x = this->pos.x - mapRoot.x - width/2;
    destRect.y = this->pos.y - mapRoot.y - dataContainer->node_yoffset[this->nodeType];
    destRect.w = width;
    destRect.h = height;

    // draw node icon
    SDL_RenderCopy(gRenderer, img, NULL, &destRect);

    // if debugging, draw bounding box
    if (debug >= DEBUG_NORMAL)
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

Coord Node::getPos()
{
    return pos;
}

bool Node::mouseDown()
{
    if (nodeStatus == NODESTATUS_HIDDEN)
    {
        return false;
    }
    else if (nodeStatus == NODESTATUS_UNOWNED)
    {
        nodeStatus = NODESTATUS_UNOWNED_SELECTED;
    }
    else if (nodeStatus == NODESTATUS_OWNED)
    {
        nodeStatus = NODESTATUS_OWNED_SELECTED;
    }
    return false;
}

bool Node::isMouseOver(Coord mapRoot)
{
    // get the width and height of the current node
    int width = 0, height = 0;
    SDL_QueryTexture(this->img, NULL, NULL, &width, &height);
    int widthOffset = width/2;
    int heightOffset = dataContainer->node_yoffset[this->nodeType];

    return (mousePos.x > this->pos.x - mapRoot.x - widthOffset &&
            mousePos.x < this->pos.x + widthOffset - mapRoot.x &&
            mousePos.y > this->pos.y - mapRoot.y - heightOffset &&
            mousePos.y < this->pos.y + (height - heightOffset) - mapRoot.y);
}

void Node::addChild(Node* child)
{
    if (children == NULL)
    {
        children = new LinkedList<Node*>();
    }
    children->addLast(child);
}

void Node::setNodeStatus(NODESTATUS ns)
{
    nodeStatus = ns;
    switch (ns)
    {
    case NODESTATUS_OWNED:
        img = dataContainer->node_owned[nodeType];
        break;
    case NODESTATUS_OWNED_SELECTED:
        img = dataContainer->node_owned_selected[nodeType];
        break;
    case NODESTATUS_UNOWNED:
        img = dataContainer->node_normal[nodeType];
        break;
    case NODESTATUS_UNOWNED_SELECTED:
        img = dataContainer->node_normal_selected[nodeType];
        break;
    default:
        break;
    }
}

NODESTATUS Node::getNodeStatus()
{
    return nodeStatus;
}

void Node::winNode()
{
    nodeStatus = NODESTATUS_OWNED;
    if (children != NULL)
    {
        Iterator<Node*> it = children->getIterator();
        while (it.hasNext())
        {
            it.next()->setNodeStatus(NODESTATUS_UNOWNED);
        }
    }
}

std::string Node::getLevelStr()
{
    return levelStr;
}
