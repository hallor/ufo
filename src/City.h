#ifndef CITY_H
#define CITY_H
#include <vector>

#include "GameModel.h"

/** Game logic state - city + alien dimension */
class CityBuildings;
class CityMap;

class City
{
public:
	City();
	~City() { clear(); }

	/** Clears old city, and generates new one on appropriate game difficulty */
	bool generate(GameDifficulty difficulty);
	void clear();

// Megalopolis
CityBuildings * m_buildings;
CityMap * m_city;

// Alien Dimension
CityBuildings * m_abuildings;
CityMap * m_acity;
};

#endif // CITY_H
