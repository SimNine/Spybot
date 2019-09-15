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
	
	// construct with texture
	GUIButtonParamaterized(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, std::string imgPath, void(*func) (T), T obj)
		: GUIButton(parent, anchor, disp, dims, [] () {}, imgPath) {
		func_ = func;
		obj_ = obj;
	}

	bool mouseDown() {
		pressed_ = true;
		return true;
	}

	bool mouseUp() {
		if (pressed_)
			func_(obj_);
		pressed_ = false;
		return true;
	}
protected:
private:
	void(*func_) (T);
	T obj_;
};
