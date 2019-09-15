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
	if (alphaInitial_ > alphaFinal_) { // if alpha is decreasing
		obj->setTransparency(alphaInitial_ - (int)(getPercentFinished()*(alphaInitial_ - alphaFinal_)) - 1);
	} else { // if alpha is increasing
		obj->setTransparency((int)(getPercentFinished()*(alphaFinal_ - alphaInitial_)) + 1);
	}

	if (getPercentFinished() >= 1.0) {
		obj->setTransparency(alphaFinal_);
	}
}