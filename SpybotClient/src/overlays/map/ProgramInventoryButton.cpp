#include "Standard.h"
#include "ProgramInventoryButton.h"

#include "Global.h"
#include "Data.h"
#include "GameOverlay.h"

ProgramInventoryButton::ProgramInventoryButton(ANCHOR a, Coord disp, PROGRAM t, GUIContainer* p)
	: GUIObject(p, a, disp, { 30, 30 }) {
	icon = _program_icons[t];
	progType = t;
}

ProgramInventoryButton::~ProgramInventoryButton() {
	// nada
}

void ProgramInventoryButton::setTransparency(int a) {
	startAlpha = a;
	endAlpha = a;
	currAlpha = a;
}

bool ProgramInventoryButton::mouseDown() {
	_currProgram = progType;
	_gameOverlay->tryPlacingProgram(_currProgram);
	return true;
}

bool ProgramInventoryButton::mouseUp() {
	return true;
}

void ProgramInventoryButton::draw() {
	SDL_RenderCopy(_renderer, icon, NULL, &bounds);

	if (_debug >= DEBUG_NORMAL)
		drawBounds();
}

void ProgramInventoryButton::tick(int t) {

}

void ProgramInventoryButton::resetBounds() {
	GUIObject::recomputePosition();
}
