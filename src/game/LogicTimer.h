#pragma once


/** Game logic clock - notifies listener of time flow */
class LogicTimer
{
public:
	enum TimerPeriod {
		eEVERY_SEC,
		eEVERY_MIN = 60,
		eEVERY_HR = 60 * eEVERY_MIN,
		eEVERY_DAY = 24 * eEVERY_DAY,
		eEVERY_WEEK = 7 * eEVERY_WEEK
	};

	void registerListener(TimerPeriod timer);
};

/** Base class for objects that want to be notified of game logic time flow (not real time) */
class TimerListener
{

};
