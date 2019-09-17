#pragma once

#include "Standard.h"

class GUIContainer;

class TimedEvent {
public:
	TimedEvent(int delay, void (*func) (void));
	virtual ~TimedEvent();

	void tick(int millisec);
	bool isDone();
	void execute();
protected:
	int delayRemaining_;
	void(*func_) (void);
};