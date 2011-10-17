#include <list>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>

#include "LogicTimer.h"

static const int periods[LogicTimer::eEVERY_END] =
{
	1,  // every second
	60,  // every minute
	60*60,  // every hour
	60*60*24, // every day
	60*60*24*7, // every week
};

// Listener at specific time
class TimerListenerPack
{
	public:
	typedef std::list<TimerListener *> ListenerList;
	typedef ListenerList::iterator ListenerListI;

	ListenerList listeners;
	// Executes action on contents
	void fire(LogicTimer::TimerPeriod period)
	{
		using std::for_each;
		using std::mem_fun;
		using std::bind2nd;

		for_each(listeners.begin(), listeners.end(),
									bind2nd(mem_fun(&TimerListener::logicTimeout), period));
	}

	void add(TimerListener * listener)
	{
		using std::count;
		if (!count(listeners.begin(), listeners.end(), listener))
			listeners.push_back(listener);
	}

	void remove(TimerListener * listener)
	{
		using std::remove;
		remove(listeners.begin(), listeners.end(), listener);
	}

};

class LogicTimer::Private
{
public:
	// Vector - for each period, list for each modulo
	std::vector< TimerListenerPack > queues;
};


LogicTimer::LogicTimer(int freq) : frequency(freq), time(0) // TODO: when it should start? at 0 == 2084? or at some point or how store date?
{
	p = new LogicTimer::Private;
	p->queues.resize(LogicTimer::eEVERY_END);
}

LogicTimer::~LogicTimer()
{
	delete p;
}

void LogicTimer::registerListener(TimerListener * listener, TimerPeriod timer)
{
	p->queues[timer].add(listener);

}

void LogicTimer::removeListener(TimerListener * listener, TimerPeriod timer)
{
	p->queues[timer].remove(listener);
}

//todo refactor crap
void LogicTimer::updateTime(int dt) //update in seconds??
{
	if (dt > frequency)
		throw std::logic_error("Can't update time above 1s");

	time += dt;
	// Process all queues, starting from "fastest"
	for (int i=0; i<eEVERY_END; ++i)
	{
		if (time % (periods[i] * frequency) == 0)
			p->queues[i].fire((TimerPeriod)i);
	}
}
