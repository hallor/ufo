#ifndef FACILITY_H
#define FACILITY_H
#include "LogicTimer.h"

/** Base class for facilities inside buildings.
	Mostly used inside X-Com buildings (but may be changed in future with Megapol/Alien mod ;) ) */
class Facility : public TimerListener
{
public:
    Facility();

    // Used for researches, healing/training of soldiers
    void logicTimeout(LogicTimer::TimerPeriod timer)  { }
};

#endif // FACILITY_H
