#pragma once


/** Game logic clock - notifies listener of time flow */
class LogicTimer
{
public:
	enum TimerPeriod {
		eEVERY_SEC,
		eEVERY_MIN,
		eEVERY_HR,
		eEVERY_DAY,
		eEVERY_WEEK
	};

	void registerListener
};

/** Base class for objects that want to be notified of game logic time flow (not real time) */
class TimerListener
{

};
