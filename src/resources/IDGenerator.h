#pragma once
#include <string>

class cIdGenerator
{
public:
    cIdGenerator(const std::string &id_base);
    
    std::string Next();

protected:

    unsigned int m_Counter;
    char m_Buffer[32];
    std::string m_Base;
};