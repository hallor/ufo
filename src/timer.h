#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>

/** Global timer */
class RealTimer
{
public:
	RealTimer() : started(SDL_GetTicks()) {}

	unsigned int delta()
	{
		unsigned int t = started - SDL_GetTicks();
		started = SDL_GetTicks();
		return t;
	}

	void restart() { started = SDL_GetTicks(); }

private:
	unsigned int started;
};

class FpsTimer
{
public:
    FpsTimer(unsigned int fps) : req_fps(fps)
    {
      if (req_fps < 1 || req_fps > 500)
        req_fps=30; // CAP
    }

    void startOfFrame()
    {
      ticks = SDL_GetTicks();
    }

    void endOfFrame()
    {
      unsigned int end = SDL_GetTicks();

      if (1000/req_fps > (end - ticks))
        SDL_Delay( (1000/req_fps) - (end - ticks));
    }

private:
  unsigned int ticks;
  unsigned int req_fps;
};

#endif // TIMER_H
