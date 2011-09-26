#include <string>
#include <fstream>
#include <map>
#include "City.h"

#include "citybuilding.h"
#include "citymap.h"

#include "utils.h"
#include "logger.h"

using namespace std;

City::City()
{
	m_city = NULL;
	m_acity = NULL;
	m_buildings = NULL;
	m_abuildings= NULL;
}


bool City::generate(GameDifficulty difficulty)
{
	clear();

	m_city = new CityMap(100, 100, 10);
	m_acity = new CityMap(100, 100, 10);

	if (!m_city || ! m_acity)
		return false;

	if (!m_city->load("XCOMA/UFODATA/CITYMAP"+Utils::stringify((int)difficulty)))
		return false;

	if (!m_acity->load("XCOMA/UFODATA/ALIENMAP"))
		return false;

	m_buildings = new CityBuildings();
	m_abuildings = new CityBuildings();

	if (!m_buildings || !m_abuildings)
		return false;

	LogInfo("Loading buildings for city...");
	{ //Load names for city
		std::ifstream bn("resources/building.nam");
		std::map<unsigned short, std::string> names;
		unsigned short i=0;
		while (bn.good())
		{
				std::string x;
				getline(bn, x);
				names[i]=x;
				i++;
		}
		if (!m_buildings->load("XCOMA/UFODATA/CITYMAP" +
													 Utils::stringify((int)difficulty) + ".BLD",
													 names))
			return false;
	}

	LogInfo("Loading buildings for alien dimension...");
	{	//Load names for alien dimension
		std::ifstream bn("resources/alienbld.typ");
		// Ask Mythos games...
		unsigned short indices[] = { 0x0, 0x1, 0x2, 0x7, 0x9, 0xC, 0xF, 0x1A, 0x22, 0x24 /*N/A*/, 0x24 };
		std::map<unsigned short, std::string> names;
		int i = 0;
		while (bn.good())
		{
			std::string x;
			getline(bn, x);
			if (i < 10)
			{
				names[indices[i]] = x;
				i++;
			}
		}
		if (!m_buildings->load("XCOMA/UFODATA/ALBUILD.BLD", names))
			return false;
	}

}

void City::clear()
{
	if (m_buildings)
		m_buildings->clear();
	if (m_abuildings)
		m_abuildings->clear();

	delete m_buildings;
	m_buildings = NULL;
	delete m_abuildings;
	m_abuildings= NULL;

	delete m_acity;
	m_acity = NULL;
	delete m_city;
	m_city = NULL;
}
