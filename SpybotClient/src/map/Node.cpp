#include "Standard.h"
#include "Node.h"

#include "Global.h"
#include "Data.h"

Node::Node(Coord p, int type, int nodeZone, int lvlId)
{
    pos = p;
    nodeType = type;
    zone = nodeZone;
    id = lvlId;
    nodeStatus = NODESTATUS_HIDDEN;
    neighbors = new LinkedList<Node*>();
    img = _node_owned[nodeType];
}

Node::~Node()
{
    delete neighbors;
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
            img = _node_owned[nodeType];
        }
        else
        {
            img = _node_normal[nodeType];
        }
    }
    else
    {
        if (nodeStatus == NODESTATUS_OWNED)
        {
            img = _node_owned_over[nodeType];
        }
        else
        {
            img = _node_normal_over[nodeType];
        }
    }

    // if this node is hightlighted
    if (nodeStatus == NODESTATUS_UNOWNED_SELECTED)
    {
        img = _node_normal_selected[nodeType];
    }
    else if (nodeStatus == NODESTATUS_OWNED_SELECTED)
    {
        img = _node_owned_selected[nodeType];
    }

    // get the width and height of the node image
    SDL_Rect destRect;
    int width, height;
    SDL_QueryTexture(img, NULL, NULL, &width, &height);

    // dump the dimensions and position into a rect
    destRect.x = this->pos.x - mapRoot.x - width/2;
    destRect.y = this->pos.y - mapRoot.y - _node_yoffset[this->nodeType];
    destRect.w = width;
    destRect.h = height;

    // draw node icon
    SDL_RenderCopy(_renderer, img, NULL, &destRect);

    // if debugging, draw bounding box
    if (_debug >= DEBUG_NORMAL)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
        SDL_RenderDrawRect(_renderer, &destRect);

        int xCenter = destRect.x + width/2;
        int yCenter = destRect.y + _node_yoffset[this->nodeType];
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
        SDL_RenderDrawLine(_renderer, xCenter-5, yCenter, xCenter+5, yCenter);
        SDL_RenderDrawLine(_renderer, xCenter, yCenter-5, xCenter, yCenter+5);
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
    int heightOffset = _node_yoffset[this->nodeType];

    return (_mousePos.x > this->pos.x - mapRoot.x - widthOffset &&
            _mousePos.x < this->pos.x + widthOffset - mapRoot.x &&
            _mousePos.y > this->pos.y - mapRoot.y - heightOffset &&
            _mousePos.y < this->pos.y + (height - heightOffset) - mapRoot.y);
}

void Node::addNeighbor(Node* neighbor)
{
    if (neighbor == this || neighbors->contains(neighbor))
        return;
    neighbors->addLast(neighbor);
    neighbor->addNeighbor(this);
}

void Node::setNodeStatus(NODESTATUS ns)
{
    nodeStatus = ns;
    switch (ns)
    {
    case NODESTATUS_OWNED:
        img = _node_owned[nodeType];
        break;
    case NODESTATUS_OWNED_SELECTED:
        img = _node_owned_selected[nodeType];
        break;
    case NODESTATUS_UNOWNED:
        img = _node_normal[nodeType];
        break;
    case NODESTATUS_UNOWNED_SELECTED:
        img = _node_normal_selected[nodeType];
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
    if (neighbors != NULL)
    {
        Iterator<Node*> it = neighbors->getIterator();
        while (it.hasNext())
        {
            Node* n = it.next();
            if (n->getNodeStatus() != NODESTATUS_OWNED)
                n->setNodeStatus(NODESTATUS_UNOWNED);
        }
    }
}

int Node::getLevelId()
{
    return id;
}

int Node::getNodeType()
{
    return nodeType;
}

int Node::getZone()
{
    return zone;
}

LinkedList<Node*>* Node::getNeighbors()
{
    return neighbors;
}
