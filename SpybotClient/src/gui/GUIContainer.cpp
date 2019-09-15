#include "Standard.h"
#include "GUIContainer.h"

#include "Global.h"

GUIContainer::GUIContainer(GUIContainer* parent, ANCHOR anch, Coord disp, Coord dims, SDL_Color col)
	: GUIObject(parent, anch, disp, dims) {
	movable_ = false;
	bkgCol_ = col;
	contents_ = new LinkedList<GUIObject*>();
}

GUIContainer::~GUIContainer() {
	// dtor
}

SDL_Color GUIContainer::getBackgroundCol() {
	return bkgCol_;
}

void GUIContainer::setBackgroundCol(SDL_Color col) {
	bkgCol_ = col;
}

LinkedList<GUIObject*>* GUIContainer::getContents() {
	return contents_;
}

// looks for and attempts to click an object within this container
bool GUIContainer::mouseDown() {
	Iterator<GUIObject*> it = contents_->getIterator();
	while (it.hasNext()) {
		GUIObject* curr = it.next();
		if (curr->isClickable() && curr->isMouseOver()) {
			return curr->mouseDown();
		}
	}

	if (isMovable() && isMouseOver()) {
		_heldContainer = this;
		parent_->removeObject(this);
		parent_->addObject(this);
		return true;
	}

	return false;
}

bool GUIContainer::mouseUp() {
	bool ret = false;
	Iterator<GUIObject*> it = contents_->getIterator();
	while (it.hasNext() && !ret) {
		GUIObject* curr = it.next();
		if (curr->isClickable() && curr->isMouseOver()) {
			ret = true;
			curr->mouseUp();
		}
	}

	setPressed(false);
	_heldContainer = NULL;
	return ret;
}

void GUIContainer::setPressed(bool p) {
	Iterator<GUIObject*> it = contents_->getIterator();
	while (it.hasNext()) {
		it.next()->setPressed(p);
	}
}

void GUIContainer::resetBounds() {
	if (parent_ == NULL) {
		setDisplacement({ 0,0 });
		setDimensions({ _screenWidth, _screenHeight });
	}

	recomputePosition();

	contents_->forEach([] (GUIObject* g) {g->resetBounds(); });
}

void GUIContainer::addObject(GUIObject* obj) {
	contents_->addFirst(obj);
}

void GUIContainer::addAllObjects(LinkedList<GUIObject*>* objList) {
	contents_->addAllFirst(objList);
}

void GUIContainer::drawBkg() {
	// draw background color if not clear
	if (bkgCol_.a != 0) {
		int newAlpha = (int)((currAlpha_ / 255.0)*bkgCol_.a);
		SDL_SetRenderDrawColor(_renderer, bkgCol_.r, bkgCol_.g, bkgCol_.b, newAlpha);
		SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(_renderer, &bounds_);
	}
}

void GUIContainer::drawContents() {
	contents_->forEachBackwards([] (GUIObject* g) {if (g->isVisible()) g->draw(); });
}

void GUIContainer::draw() {
	drawBkg();
	drawContents();

	if (_debug >= DEBUG_NORMAL)
		drawBounds();
}

void GUIContainer::setTransparency(int a) {
	if (a > 255)
		currAlpha_ = 255;
	else if (a < 0)
		currAlpha_ = 0;
	else
		currAlpha_ = a;

	Iterator<GUIObject*> it = contents_->getIterator();
	while (it.hasNext()) {
		it.next()->setTransparency(currAlpha_);
	}
}

void GUIContainer::tick(int ms) {
	processEffects(ms);

	Iterator<GUIObject*> it = contents_->getIterator();
	while (it.hasNext()) {
		it.next()->tick(ms);
	}
}

void GUIContainer::incDisplacement(Coord delta) {
	displacement_ = delta + displacement_;
	resetBounds();
}

bool GUIContainer::isMovable() {
	if (movable_ && parent_ != NULL && currAlpha_ > 0)
		return true;
	else
		return false;
}

void GUIContainer::setMovable(bool m) {
	movable_ = m;
}

void GUIContainer::removeObject(GUIObject* o) {
	contents_->remove(o);
}
