#pragma once


/** Game logic clock - notifies listener of time flow */
class TimerListener;

/** TODO: now wery simple (task is called at every min :00 and so on -
not exactly 1 min after start

It probably doesn't matter as original game handles it in the same way ;)
*/
class LogicTimer
{
public:
	enum TimerPeriod {
		eEVERY_SEC = 0,
		eEVERY_MIN,
		eEVERY_HR,
		eEVERY_DAY,
		eEVERY_WEEK,
		eEVERY_END
	};

	LogicTimer(int freq = 10); // TODO: when it should start? at 0 == 2084? or at some point or how store date?
	~LogicTimer();

	// Register object for (periodic) calls starting from now
	void registerListener(TimerListener * listener, TimerPeriod timer);
	void removeListener(TimerListener * listener, TimerPeriod timer);

	void updateTime(int dt); //update in 1/10 seconds??

private:
	class Private;
	Private * p;
	int frequency;
	unsigned long long time;
};

/** Base class for objects that want to be notified of game logic time flow (not real time) */
class TimerListener
{
public:
	// as a parameter we get the timer that expired
	virtual void logicTimeout(LogicTimer::TimerPeriod timer) = 0;
};
