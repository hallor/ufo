#include "Building.h"

Building::Building()
{
}

Building::~Building()
{

}

// Can this vehicle land inside this building ??
bool Building::canLand(const Vehicle * v) const
{
	return false;
}

// Can this unit enter this building
bool Building::canEnter(const Unit * u) const
{
	return false;
}

// When unit enters/exits building it's placed somewhere (+ it's added to building object list).
// If building is then destroyed, units/vehicles may get damaged
void Building::enter(const Unit * u)
{
	return;
}
void Building::enter(const Vehicle * v)
{
	return;
}

void Building::exit(const Unit * u)
{
	return;
}
void Building::exit(const Vehicle * v)
{
	return;
}

// Returns places where flying vehicles can land
const CityCoordV & Building::landingZones() const
{
	return CityCoordV();
}

// Returns places where ground vehicles can enter building
const CityCoordV & Building::taxiZones() const
{
	return CityCoordV();
}

// Pedestrian tube entrances
const CityCoordV & Building::tubeZones() const
{
	return CityCoordV();
}
