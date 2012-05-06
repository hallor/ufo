#include "Timer.h"
#include <SDL\SDL.h>
#include <iostream>

cTimer::cTimer()
{
    memset(this, 0, sizeof(*this));
}

void cTimer::Start()
{
    m_StartTime = SDL_GetTicks();
    m_PreviousTime = m_StartTime;
    m_CurrentTime = m_StartTime;
}

void cTimer::Tick()
{
    m_PreviousTime = m_CurrentTime;
    m_CurrentTime = SDL_GetTicks();
}

float cTimer::GetTimeDelta() const
{
    return (m_CurrentTime - m_PreviousTime) / 1000.0f;
}

float cTimer::GetTime() const
{
    return (m_CurrentTime - m_StartTime) / 1000.0f;
}