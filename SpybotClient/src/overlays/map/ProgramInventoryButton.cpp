#include "Standard.h"
#include "ProgramInventoryButton.h"

#include "Global.h"
#include "Data.h"
#include "GameOverlay.h"

ProgramInventoryButton::ProgramInventoryButton(ANCHOR a, Coord disp, PROGRAM t, GUIContainer* p)
	: GUIObject(p, a, disp, { 30, 30 }) {
	icon_ = _program_icons[t];
	progType_ = t;
}

ProgramInventoryButton::~ProgramInventoryButton() {
	// nada
}

void ProgramInventoryButton::setTransparency(int a) {
	currAlpha_ = a;
}

bool ProgramInventoryButton::mouseDown() {
	_currProgram = progType_;
	_gameOverlay->tryPlacingProgram(_currProgram);
	return true;
}

bool ProgramInventoryButton::mouseUp() {
	return true;
}

void ProgramInventoryButton::draw() {
	SDL_RenderCopy(_renderer, icon_, NULL, &bounds_);

	if (_debug >= DEBUG_NORMAL)
		drawBounds();
}

void ProgramInventoryButton::tick(int t) {

}

void ProgramInventoryButton::resetBounds() {
	GUIObject::recomputePosition();
}
