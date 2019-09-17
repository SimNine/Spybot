#pragma once

#include "Standard.h"
#include "GUIEffect.h"

class GUIObject;

class GUIEffectFade : public GUIEffect {
public:
	GUIEffectFade(int delay, int duration, Uint8 alphaInitial, Uint8 alphaFinal);
	virtual ~GUIEffectFade();

	void affectObject(GUIObject* object);
	int getAlpha();
protected:
	int alphaInitial_;
	int alphaFinal_;
};