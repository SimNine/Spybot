#include "Standard.h"
#include "GUIObject.h"

#include "GUIContainer.h"
#include "Global.h"
#include "GUIEffect.h"

GUIObject::GUIObject(GUIContainer* parent, ANCHOR anchorPoint, Coord disp, Coord dims) {
	anchor_ = anchorPoint;
	displacement_ = disp;
	pressed_ = false;

	effectList_ = new LinkedList<GUIEffect*>();
	currAlpha_ = 255;

	parent_ = parent;
	setDimensions(dims);
	setDisplacement(disp);
}

GUIObject::~GUIObject() {
	while (effectList_->getLength() > 0) {
		delete effectList_->poll();
	}
	delete effectList_;
}

void GUIObject::setDisplacement(Coord disp) {
	displacement_ = disp;
	recomputePosition();
}

Coord GUIObject::getDisplacement() {
	return displacement_;
}

void GUIObject::setDimensions(Coord dims) {
	bounds_.w = dims.x;
	bounds_.h = dims.y;
	recomputePosition();
}

Coord GUIObject::getDimensions() {
	return { bounds_.w, bounds_.h };
}

void GUIObject::recomputePosition() {
	bounds_.x = getXAnchor() + displacement_.x;
	bounds_.y = getYAnchor() + displacement_.y;
}

int GUIObject::getXAnchor() {
	if (parent_ == NULL) {
		return 0;
	}

	switch (anchor_) {
	case ANCHOR_NORTHEAST:
	case ANCHOR_EAST:
	case ANCHOR_SOUTHEAST:
		return parent_->getBounds().x + parent_->getBounds().w - bounds_.w;
	case ANCHOR_NORTH:
	case ANCHOR_CENTER:
	case ANCHOR_SOUTH:
		return parent_->getBounds().x + parent_->getBounds().w / 2 - bounds_.w / 2;
	case ANCHOR_NORTHWEST:
	case ANCHOR_WEST:
	case ANCHOR_SOUTHWEST:
		return parent_->getBounds().x;
	}

	return -1;
}

int GUIObject::getYAnchor() {
	if (parent_ == NULL) {
		return 0;
	}

	switch (anchor_) {
	case ANCHOR_NORTHWEST:
	case ANCHOR_NORTH:
	case ANCHOR_NORTHEAST:
		return parent_->getBounds().y;
	case ANCHOR_WEST:
	case ANCHOR_CENTER:
	case ANCHOR_EAST:
		return parent_->getBounds().y + parent_->getBounds().h / 2 - bounds_.h / 2;
	case ANCHOR_SOUTHEAST:
	case ANCHOR_SOUTH:
	case ANCHOR_SOUTHWEST:
		return parent_->getBounds().y + parent_->getBounds().h - bounds_.h;
	}

	return -1;
}

SDL_Rect GUIObject::getBounds() {
	return bounds_;
}

bool GUIObject::isMouseOver() {
	if (_mousePos.x >= bounds_.x &&
		_mousePos.x < bounds_.x + bounds_.w &&
		_mousePos.y >= bounds_.y &&
		_mousePos.y < bounds_.y + bounds_.h) {
		return true;
	} else {
		return false;
	}
}

bool GUIObject::isVisible() {
	return (currAlpha_ > 0);
}

bool GUIObject::isClickable() {
	return (currAlpha_ == 255);
}

void GUIObject::setPressed(bool b) {
	pressed_ = b;
}

void GUIObject::drawBounds() {
	if (isMouseOver()) {
		if (pressed_) {
			SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
		} else {
			SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
		}
	} else {
		if (pressed_) {
			SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
		} else {
			SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		}
	}
	SDL_RenderDrawRect(_renderer, &bounds_);
}

int GUIObject::getTransparency() {
	return currAlpha_;
}

void GUIObject::processEffects(int ms) {
	Iterator<GUIEffect*> it = effectList_->getIterator();
	LinkedList<GUIEffect*>* effectsToRemove = new LinkedList<GUIEffect*>();
	while (it.hasNext()) {
		GUIEffect* curr = it.next();

		curr->tick(ms);

		if (curr->isActive()) {
			curr->affectObject(this);
		}

		if (curr->isDead()) {
			effectsToRemove->addFirst(curr);
		}
	}

	while (effectsToRemove->getLength() > 0) {
		GUIEffect* curr = effectsToRemove->poll();
		effectList_->remove(curr);
		delete curr;
	}
	delete effectsToRemove;
}

void GUIObject::addEffect(GUIEffect* e) {
	effectList_->addFirst(e);
}

void GUIObject::removeAllEffects(bool finishFirst) {
	while (effectList_->getLength() > 0) {
		GUIEffect* currEffect = effectList_->poll();
		if (finishFirst) {
			currEffect->tick(INT_MAX);
			currEffect->affectObject(this);
		}
		delete currEffect;
	}
}