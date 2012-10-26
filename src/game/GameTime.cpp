#include "game_pch.h"
#include "GameTime.h"

GameTime::GameTime()
: m_CurrentGameSpeed(EGameSpeed::SpeedPaused)
, m_TimeDelta(0.0f)
, m_CurrentTime(0.0f)
{

}

void GameTime::Advance(float real_time_dt)
{
    CHECK(real_time_dt >= 0.0f);

    m_TimeDelta = RealToGameTime(real_time_dt, EGameSpeed::Speed5s) / 5.0f;
    m_CurrentTime += RealToGameTime(real_time_dt, GetGameSpeed());
}

sHMSTime GameTime::GetTimeHMSFormat() const
{
    sHMSTime out;
    int time = (int)floorf(GetTime());

    out.m_Seconds = time % 60;
    time /= 60;
    out.m_Minutes = time % 60;
    time /= 60;
    out.m_Hours = time % 24;

    return out;
}

float GameTime::RealToGameTime(float real_time, EGameSpeed::TYPE game_speed) const
{
    int mul = game_speed;

    return real_time * game_speed;
}

void GameTime::Reset()
{
    m_CurrentTime = 0.0f;
    m_TimeDelta = 0.0f;
    m_CurrentGameSpeed = EGameSpeed::SpeedPaused;
}