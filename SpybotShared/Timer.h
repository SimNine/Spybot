#ifndef TIMER_H
#define TIMER_H

class Timer
{
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
        unsigned int mStartTicks;

        //The ticks stored when the timer was paused
        unsigned int mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
};

#endif // TIMER_H
