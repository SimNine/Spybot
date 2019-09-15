#include "Standard.h"
#include "Timer.h"

Timer::Timer() {
	//Initialize the variables
	mStartTicks_ = 0;
	mPausedTicks_ = 0;

	mPaused_ = false;
	mStarted_ = false;
}

Timer::~Timer() {
	//dtor
}

void Timer::start() {
	//Start the timer
	mStarted_ = true;

	//Unpause the timer
	mPaused_ = false;

	//Get the current clock time
	mStartTicks_ = SDL_GetTicks();
	mPausedTicks_ = 0;
}

void Timer::stop() {
	//Stop the timer
	mStarted_ = false;

	//Unpause the timer
	mPaused_ = false;

	//Clear tick variables
	mStartTicks_ = 0;
	mPausedTicks_ = 0;
}

void Timer::pause() {
	//If the timer is running and isn't already paused
	if (mStarted_ && !mPaused_) {
		//Pause the timer
		mPaused_ = true;

		//Calculate the paused ticks
		mPausedTicks_ = SDL_GetTicks() - mStartTicks_;
		mStartTicks_ = 0;
	}
}

void Timer::unpause() {
	//If the timer is running and paused
	if (mStarted_ && mPaused_) {
		//Unpause the timer
		mPaused_ = false;

		//Reset the starting ticks
		mStartTicks_ = SDL_GetTicks() - mPausedTicks_;

		//Reset the paused ticks
		mPausedTicks_ = 0;
	}
}

Uint32 Timer::getTicks() {
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted_) {
		//If the timer is paused
		if (mPaused_) {
			//Return the number of ticks when the timer was paused
			time = mPausedTicks_;
		} else {
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks_;
		}
	}

	return time;
}

bool Timer::isStarted() {
	//Timer is running and paused or unpaused
	return mStarted_;
}

bool Timer::isPaused() {
	//Timer is running and paused
	return mPaused_ && mStarted_;
}
