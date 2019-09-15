#include "Standard.h"
#include "Node.h"

#include "Global.h"
#include "Data.h"

Node::Node(Coord pos, int type, int zone, int lvlId) {
	pos_ = pos;
	nodeType_ = type;
	zone_ = zone;
	id_ = lvlId;
	nodeStatus_ = NODESTATUS_HIDDEN;
	neighbors_ = new LinkedList<Node*>();
	img_ = _node_owned[nodeType_];
}

Node::~Node() {
	delete neighbors_;
}

void Node::draw(Coord mapRoot) {
	// if this node is not available yet
	if (nodeStatus_ == NODESTATUS_HIDDEN) {
		return;
	}

	// if the mouse is over this node
	if (!isMouseOver(mapRoot)) {
		if (nodeStatus_ == NODESTATUS_OWNED) {
			img_ = _node_owned[nodeType_];
		} else {
			img_ = _node_normal[nodeType_];
		}
	} else {
		if (nodeStatus_ == NODESTATUS_OWNED) {
			img_ = _node_owned_over[nodeType_];
		} else {
			img_ = _node_normal_over[nodeType_];
		}
	}

	// if this node is hightlighted
	if (nodeStatus_ == NODESTATUS_UNOWNED_SELECTED) {
		img_ = _node_normal_selected[nodeType_];
	} else if (nodeStatus_ == NODESTATUS_OWNED_SELECTED) {
		img_ = _node_owned_selected[nodeType_];
	}

	// get the width and height of the node image
	SDL_Rect destRect;
	int width, height;
	SDL_QueryTexture(img_, NULL, NULL, &width, &height);

	// dump the dimensions and position into a rect
	destRect.x = this->pos_.x - mapRoot.x - width / 2;
	destRect.y = this->pos_.y - mapRoot.y - _node_yoffset[this->nodeType_];
	destRect.w = width;
	destRect.h = height;

	// draw node icon
	SDL_RenderCopy(_renderer, img_, NULL, &destRect);

	// if debugging, draw bounding box
	if (_debug >= DEBUG_NORMAL) {
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
		SDL_RenderDrawRect(_renderer, &destRect);

		int xCenter = destRect.x + width / 2;
		int yCenter = destRect.y + _node_yoffset[this->nodeType_];
		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
		SDL_RenderDrawLine(_renderer, xCenter - 5, yCenter, xCenter + 5, yCenter);
		SDL_RenderDrawLine(_renderer, xCenter, yCenter - 5, xCenter, yCenter + 5);
	}
}

Coord Node::getPos() {
	return pos_;
}

bool Node::mouseDown() {
	if (nodeStatus_ == NODESTATUS_HIDDEN) {
		return false;
	} else if (nodeStatus_ == NODESTATUS_UNOWNED) {
		nodeStatus_ = NODESTATUS_UNOWNED_SELECTED;
	} else if (nodeStatus_ == NODESTATUS_OWNED) {
		nodeStatus_ = NODESTATUS_OWNED_SELECTED;
	}
	return false;
}

bool Node::isMouseOver(Coord mapRoot) {
	// get the width and height of the current node
	int width = 0, height = 0;
	SDL_QueryTexture(this->img_, NULL, NULL, &width, &height);
	int widthOffset = width / 2;
	int heightOffset = _node_yoffset[this->nodeType_];

	return (_mousePos.x > this->pos_.x - mapRoot.x - widthOffset &&
		_mousePos.x < this->pos_.x + widthOffset - mapRoot.x &&
		_mousePos.y > this->pos_.y - mapRoot.y - heightOffset &&
		_mousePos.y < this->pos_.y + (height - heightOffset) - mapRoot.y);
}

void Node::addNeighbor(Node* neighbor) {
	if (neighbor == this || neighbors_->contains(neighbor))
		return;
	neighbors_->addLast(neighbor);
	neighbor->addNeighbor(this);
}

void Node::setNodeStatus(NODESTATUS ns) {
	nodeStatus_ = ns;
	switch (ns) {
	case NODESTATUS_OWNED:
		img_ = _node_owned[nodeType_];
		break;
	case NODESTATUS_OWNED_SELECTED:
		img_ = _node_owned_selected[nodeType_];
		break;
	case NODESTATUS_UNOWNED:
		img_ = _node_normal[nodeType_];
		break;
	case NODESTATUS_UNOWNED_SELECTED:
		img_ = _node_normal_selected[nodeType_];
		break;
	default:
		break;
	}
}

NODESTATUS Node::getNodeStatus() {
	return nodeStatus_;
}

void Node::winNode() {
	nodeStatus_ = NODESTATUS_OWNED;
	if (neighbors_ != NULL) {
		Iterator<Node*> it = neighbors_->getIterator();
		while (it.hasNext()) {
			Node* n = it.next();
			if (n->getNodeStatus() != NODESTATUS_OWNED)
				n->setNodeStatus(NODESTATUS_UNOWNED);
		}
	}
}

int Node::getLevelId() {
	return id_;
}

int Node::getNodeType() {
	return nodeType_;
}

int Node::getZone() {
	return zone_;
}

LinkedList<Node*>* Node::getNeighbors() {
	return neighbors_;
}
