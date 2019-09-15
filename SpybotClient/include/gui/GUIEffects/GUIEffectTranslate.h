#pragma once

#include "Standard.h"
#include "GUIEffect.h"

class GUIObject;

class GUIEffectTranslate : public GUIEffect {
public:
	GUIEffectTranslate(int delay, int duration, Coord coordsInitial, Coord coordFinal);
	virtual ~GUIEffectTranslate();

	void affectObject(GUIObject* object);
protected:
	Coord coordsInitial_;
	Coord coordsFinal_;
};