#include "LogicManager.h"

#include "City.h"

LogicManager::LogicManager()
{
	m_city = new City();
}

LogicManager::~LogicManager()
{
	delete m_city;
}

void LogicManager::init()
{
	m_city->generate(EGameDifficulty::Novice);
}
