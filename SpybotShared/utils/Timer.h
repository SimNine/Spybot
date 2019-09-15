#pragma once

class Timer {
public:
	Timer();
	virtual ~Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	unsigned int getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();
protected:
private:
	//The clock time when the timer started
	unsigned int mStartTicks_;

	//The ticks stored when the timer was paused
	unsigned int mPausedTicks_;

	//The timer status
	bool mPaused_;
	bool mStarted_;
};