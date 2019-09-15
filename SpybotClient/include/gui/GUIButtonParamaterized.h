#pragma once

#include "Standard.h"
#include "GUIButton.h"

class GUIContainer;

template<class T> class GUIButtonParamaterized : public GUIButton {
public:
	// construct with single string
	GUIButtonParamaterized(GUIContainer* parent, ANCHOR anchor, Coord disp, std::string text, int fontSize, void(*func) (T), T obj)
		: GUIButton(parent, anchor, disp, text, fontSize, [] () {}) {
		func_ = func;
		obj_ = obj;
	}

	bool mouseDown() {
		pressed = true;
		return true;
	}

	bool mouseUp() {
		if (pressed)
			func_(obj_);
		pressed = false;
		return true;
	}
protected:
private:
	void(*func_) (T);
	T obj_;
};
