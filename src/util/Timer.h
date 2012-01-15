#pragma once

class cTimer
{
public:
    cTimer();

    void Start();
    void Tick();

    float GetTimeDelta() const;
    float GetTime() const;
protected:

    unsigned int m_StartTime;
    unsigned int m_PreviousTime;
    unsigned int m_CurrentTime;
};