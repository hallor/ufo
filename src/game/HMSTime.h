#pragma once

struct sHMSTime
{
    sHMSTime()
    {
        m_Hours = m_Minutes = m_Seconds = 0; 
    }

    int m_Hours;
    int m_Minutes;
    int m_Seconds;
};