#include "Standard.h"
#include "GUIEffectFade.h"

#include "GUIObject.h"

GUIEffectFade::GUIEffectFade(int delay, int duration, Uint8 alphaInit, Uint8 alphaFinal)
	: GUIEffect(delay, duration) {
	alphaInitial_ = alphaInit;
	alphaFinal_ = alphaFinal;
}

GUIEffectFade::~GUIEffectFade() {
	//dtor
}

void GUIEffectFade::affectObject(GUIObject* obj) {
	obj->setTransparency((int)getAlpha());

	if (getPercentFinished() >= 1.0) {
		obj->setTransparency((int)alphaFinal_);
	}
}

int GUIEffectFade::getAlpha() {
	int newAlpha;
	if (alphaInitial_ > alphaFinal_) { // if alpha is decreasing
		newAlpha = (alphaInitial_ - (int)(getPercentFinished()*(alphaInitial_ - alphaFinal_)) - 1);
	} else { // if alpha is increasing
		newAlpha = ((int)(getPercentFinished()*(alphaFinal_ - alphaInitial_)) + 1);
	}

	if (newAlpha > 255)
		newAlpha = 255;
	if (newAlpha < 0)
		newAlpha = 0;

	return newAlpha;
}