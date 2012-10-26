#pragma once
#include "HMSTime.h"


class GameTime
{
public:
    GameTime();

    float GetTime() const { return m_CurrentTime; }// returns time in game seconds
    float GetTimeDelta() const { return m_TimeDelta * GetGameSpeed(); } // returns time delta in game seconds;

    sHMSTime GetTimeHMSFormat() const;

    void Advance(float real_time_dt);

    float RealToGameTime(float real_time, EGameSpeed::TYPE game_speed) const;

    void SetGameSpeed(EGameSpeed::TYPE speed) { m_CurrentGameSpeed = speed; }
    EGameSpeed::TYPE GetGameSpeed() const { return m_CurrentGameSpeed; }

    void Reset();

protected:

    EGameSpeed::TYPE m_CurrentGameSpeed;

    float m_CurrentTime; // current time in game seconds
    float m_TimeDelta; // time delta in game seconds 
};