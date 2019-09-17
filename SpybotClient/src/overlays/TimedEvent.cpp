#include "Standard.h"
#include "TimedEvent.h"

#include "GUIContainer.h"

TimedEvent::TimedEvent(int delay, void (*func) (void)) {
	delayRemaining_ = delay;
	func_ = func;
}

TimedEvent::~TimedEvent() {
	//dtor
}

void TimedEvent::tick(int ms) {
	if (delayRemaining_ > 0) {
		delayRemaining_ -= ms;
	}
}

bool TimedEvent::isDone() {
	return (delayRemaining_ < 0);
}

void TimedEvent::execute() {
	func_();
}