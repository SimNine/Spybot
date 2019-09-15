#include "Standard.h"
#include "GUIContainer.h"

#include "Global.h"

GUIContainer::GUIContainer(ANCHOR anchorPoint, Coord disp, Coord dims, GUIContainer* parent, SDL_Texture* bkg)
	: GUIObject(anchorPoint, disp, dims, parent) {
	movable = true;
	bkgImg = bkg;
	bkgCol = NULL;
	contents = new LinkedList<GUIObject*>();
}

GUIContainer::GUIContainer(ANCHOR anch, Coord disp, GUIContainer* parent, SDL_Texture* bkg)
	: GUIObject(anch, disp, { 0, 0 }, parent) {
	movable = true;
	bkgImg = bkg;
	bkgCol = NULL;
	SDL_QueryTexture(bkg, NULL, NULL, &bounds.w, &bounds.h);
	contents = new LinkedList<GUIObject*>();
}

GUIContainer::GUIContainer(ANCHOR anch, Coord disp, Coord dims, GUIContainer* parent, SDL_Color col)
	: GUIObject(anch, disp, dims, parent) {
	movable = true;
	bkgImg = NULL;
	bkgCol = new SDL_Color(col);
	contents = new LinkedList<GUIObject*>();
}

GUIContainer::~GUIContainer() {
	if (bkgCol != NULL)
		delete bkgCol;
}

SDL_Texture* GUIContainer::getBackgroundImg() {
	return bkgImg;
}

SDL_Color GUIContainer::getBackgroundCol() {
	return *bkgCol;
}

void GUIContainer::setBackgroundImg(SDL_Texture* bkg) {
	bkgImg = bkg;
	if (bkgCol != NULL) {
		delete bkgCol;
		bkgCol = NULL;
	}
}

void GUIContainer::setBackgroundCol(SDL_Color col) {
	bkgCol = new SDL_Color(col);
	bkgImg = NULL;
}

LinkedList<GUIObject*>* GUIContainer::getContents() {
	return contents;
}

// looks for and attempts to click an object within this container
bool GUIContainer::mouseDown() {
	Iterator<GUIObject*> it = contents->getIterator();
	while (it.hasNext()) {
		GUIObject* curr = it.next();
		if (curr->isVisible() && curr->isMouseOver()) {
			return curr->mouseDown();
		}
	}

	if (isMovable() && isMouseOver()) {
		_heldContainer = this;
		parent->removeObject(this);
		parent->addObject(this);
		return true;
	}

	return false;
}

bool GUIContainer::mouseUp() {
	bool ret = false;
	Iterator<GUIObject*> it = contents->getIterator();
	while (it.hasNext() && !ret) {
		GUIObject* curr = it.next();
		if (curr->isVisible() && curr->isMouseOver()) {
			ret = true;
			curr->mouseUp();
		}
	}

	setPressed(false);
	_heldContainer = NULL;
	return ret;
}

void GUIContainer::setPressed(bool p) {
	Iterator<GUIObject*> it = contents->getIterator();
	while (it.hasNext()) {
		it.next()->setPressed(p);
	}
}

void GUIContainer::resetBounds() {
	recomputePosition();

	if (parent == NULL) {
		setBounds({ 0, 0 }, { _SCREEN_WIDTH, _SCREEN_HEIGHT });
	}

	contents->forEach([] (GUIObject* g) {g->resetBounds(); });
}

void GUIContainer::addObject(GUIObject* obj) {
	contents->addFirst(obj);
}

void GUIContainer::addAllObjects(LinkedList<GUIObject*>* objList) {
	contents->addAllFirst(objList);
}

void GUIContainer::drawBkg() {
	// if this container has no parent (and is therefore an overlay)
	if (parent == NULL) {
		//Clear screen
		SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(_renderer);
	}

	if (bkgCol != NULL) {
		// draw background color
		SDL_SetRenderDrawColor(_renderer, bkgCol->r, bkgCol->g, bkgCol->b, bkgCol->a);
		SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(_renderer, &bounds);
	} else if (bkgImg != NULL) {
		// draw background image
		SDL_RenderCopy(_renderer, bkgImg, NULL, &bounds);
	}
}

void GUIContainer::drawContents() {
	contents->forEachBackwards([] (GUIObject* g) {if (g->isVisible()) g->draw(); });
}

void GUIContainer::draw() {
	drawBkg();
	drawContents();

	if (_debug >= DEBUG_NORMAL) drawBounds();
}

void GUIContainer::setTransparency(int a) {
	if (a > 255) currAlpha = 255;
	else if (a < 0) currAlpha = 0;
	else currAlpha = a;

	Iterator<GUIObject*> it = contents->getIterator();
	while (it.hasNext()) {
		it.next()->setTransparency(currAlpha);
	}
}

void GUIContainer::tick(int ms) {
	fadeStep(ms);

	Iterator<GUIObject*> it = contents->getIterator();
	while (it.hasNext()) {
		it.next()->tick(ms);
	}
}

void GUIContainer::incDisplacement(Coord delta) {
	displacement = delta + displacement;
	resetBounds();
}

bool GUIContainer::isMovable() {
	if (movable && parent != NULL && currAlpha > 0) return true;
	else return false;
}

void GUIContainer::setMovable(bool m) {
	movable = m;
}

void GUIContainer::removeObject(GUIObject* o) {
	contents->remove(o);
}
