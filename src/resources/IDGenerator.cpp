#include "IDGenerator.h"

cIdGenerator::cIdGenerator(const std::string &base)
{
    m_Counter = 0;
    m_Base = base;
};

std::string cIdGenerator::Next()
{
    sprintf_s(m_Buffer, sizeof(m_Buffer), "@%i", m_Counter);
    return (m_Base + m_Buffer);
};