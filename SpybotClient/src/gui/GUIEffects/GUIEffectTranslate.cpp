#include "Standard.h"
#include "GUIEffectTranslate.h"

#include "GUIObject.h"

GUIEffectTranslate::GUIEffectTranslate(int delay, int duration, Coord coordInit, Coord coordFinal)
	: GUIEffect(delay, duration) {
	coordsInitial_ = coordInit;
	coordsFinal_ = coordFinal;
}

GUIEffectTranslate::~GUIEffectTranslate() {
	//dtor
}

void GUIEffectTranslate::affectObject(GUIObject* obj) {
	Coord newCoord;

	if (coordsInitial_.x > coordsFinal_.x) { // if alpha is decreasing
		newCoord.x = coordsInitial_.x - (int)(getPercentFinished()*(coordsInitial_.x - coordsFinal_.x)) - 1;
	} else { // if alpha is increasing
		newCoord.x = coordsInitial_.x + (int)(getPercentFinished()*(coordsFinal_.x - coordsInitial_.x)) + 1;
	}
	if (coordsInitial_.y > coordsFinal_.y) { // if alpha is decreasing
		newCoord.y = coordsInitial_.y - (int)(getPercentFinished()*(coordsInitial_.y - coordsFinal_.y)) - 1;
	} else { // if alpha is increasing
		newCoord.y = coordsInitial_.y + (int)(getPercentFinished()*(coordsFinal_.y - coordsInitial_.y)) + 1;
	}

	if (getPercentFinished() >= 1.0) {
		newCoord = coordsFinal_;
	}

	obj->setDisplacement(newCoord);
	obj->resetBounds();
}